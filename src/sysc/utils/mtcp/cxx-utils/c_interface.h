/*
 * c_interface.h
 *
 *  Created on: Sep 5, 2012
 *      Author: garin
 */

#ifdef __cplusplus
extern "C" {
#endif

char working_dir[PATH_MAX];
char program_name[PATH_MAX];

// don't foget to free the return pointer after used
char* get_next_ckpt_img_path(int ckpt_img_num_limit);

#ifdef __cplusplus
}
#endif
