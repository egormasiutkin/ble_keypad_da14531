/**
 ****************************************************************************************
 *
 * @file user_gamepad.h
 *
 * @brief Gamepad controls implementation header code.
 *
 * Copyright (c) 2015-2021 Renesas Electronics Corporation and/or its affiliates
 * The MIT License (MIT)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 ****************************************************************************************
 */
#ifndef _USER_GAMEPAD_H_
#define _USER_GAMEPAD_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 #include "rwip_config.h" 
 #include "rwble_config.h"
 #include "app.h"   
 #include "ke_msg.h"
 #include "user_periph_setup.h"
 
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */

#define DIGITIZER_MAX_RANGE 0x1FFF
#define MAX_MULTITOUCH      4
#define AXIS_UPDATE_PER		10   //
#define R_DEADZONE				8    //out of 100
#define LS_ADC_SAMPLE_MIN       0
#define ADC_SAMPLE_MAX				1860

#define CFG_USE_DIGITIZER   (0)
#define CFG_USE_JOYSTICKS		(1)

/*
 * FUNCTION CLAIM
 ****************************************************************************************
 */
void user_gamepad_init(void);
void user_gamepad_enable_buttons(void);
void user_gamepad_config_digitizer(void);
void user_gamepad_toggle_axis_polling(bool on);
void app_hid_gamepad_event_handler(ke_msg_id_t const msgid,
                                         void const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id);
#endif
