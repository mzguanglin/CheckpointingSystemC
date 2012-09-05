/*
 * c_interface.h
 *
 *  Created on: Sep 5, 2012
 *      Author: garin
 */

#ifdef __cplusplus
extern "C" {
#endif

// don't foget to free the return pointer after used
char* get_next_ckpt_img_path(int ckpt_img_num_limit);

#ifdef __cplusplus
}
#endif
