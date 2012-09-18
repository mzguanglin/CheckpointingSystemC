/*
 * CkptDirectoryManager.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: garin
 */

#include <iostream>
#include <climits>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
using namespace std;

#include "c_interface.h"

class CkptDirectoryManager {

public:

	~CkptDirectoryManager() {
		ckpt_img_entries.clear();
	}

	// don't foget to free the return pointer after used
	char* next_ckpt_img_path(unsigned int ckpt_img_num_limited) {
		ckpt_img_entries.clear();

		fetch_working_dir_and_prog_name();

		fetch_ckpt_imgs_info();

		sort( ckpt_img_entries.begin(), ckpt_img_entries.end());

		if (ckpt_img_entries.size() >= ckpt_img_num_limited) {
			delete_head_imgs(ckpt_img_entries.size() - ckpt_img_num_limited + 1);
		}

		if (ckpt_img_entries.size() > 0) {
			return getCkptImgPath((ckpt_img_entries[ckpt_img_entries.size()-1] + 1));
		}
		else
			return getCkptImgPath(1);
	}

private:

	void delete_head_imgs(int num) {
		for (int i=0; i<num; i++) {
			char* path = getCkptImgPath(ckpt_img_entries[i]);
			remove(path);
			free(path);
		}
	}

	char* getCkptImgPath(int ckpt_id) {
		stringstream id_ss;
		id_ss<<working_dir<<program_name<<".ckpt."<<ckpt_id;
		char *p = (char *)malloc(id_ss.str().size()+1);
		strcpy(p, id_ss.str().c_str());

		return p;
	}

	void fetch_ckpt_imgs_info() {
		stringstream ss;
		ss<<"^"<<program_name<<".ckpt.";
		string pattern = ss.str();
			regex_t regex_pattern;
			regmatch_t pmatch;

			if ( regcomp(&regex_pattern, pattern.c_str(), REG_ICASE|REG_EXTENDED) != 0 ) {
				cout<<"cannot compile regex pattern"<<endl;
				return;
			}

			DIR *dp;
			struct dirent *entry;

			if((dp = opendir(working_dir)) == NULL) {
			        fprintf(stderr,"cannot open directory: %s\n", working_dir);
			        return;
			}
			chdir(working_dir);
			while((entry = readdir(dp)) != NULL) {
				if ( regexec(&regex_pattern, entry->d_name, 1, &pmatch, NULL) == 0 ) {

					//get ckpt id
					int ckpt_id = atoi(&entry->d_name[pmatch.rm_eo]);
					ckpt_img_entries.push_back(ckpt_id);
				}
			}

			closedir(dp);

	}

	void fetch_working_dir_and_prog_name () {
		//get working dir & program name
		if (working_dir[0] != '\0' && program_name[0] != '\0') //fetch once!
			return;

		if ( -1 != readlink( "/proc/self/exe", program_name, _POSIX_PATH_MAX ) )
		{
			// now we get a string like /usr/bin/programname, we need convert it to work_dir and prog_name
			char* p1, *p2;
			p1 = p2 = program_name;
			do {
				p1 = p2;
				p2++;
				p2 = strchr(p2, '/');
			} while (p2 != NULL);
			p1++;

			strncpy(working_dir, program_name, p1 - program_name);
			strcpy(program_name, p1);
		}
	}


public:
private:
	vector<int> ckpt_img_entries;

};

char* get_next_ckpt_img_path(int ckpt_img_num_limit) {
	CkptDirectoryManager man;
	return  man.next_ckpt_img_path(ckpt_img_num_limit);
}
