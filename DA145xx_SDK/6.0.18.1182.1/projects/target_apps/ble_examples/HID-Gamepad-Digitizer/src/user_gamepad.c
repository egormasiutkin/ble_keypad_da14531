/**
 ****************************************************************************************
 *
 * @file user_gamepad.c
 *
 * @brief Gamepad control implementation source code.
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
 
 /*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
 #include "app_hogpd.h"
 #include "app_hogpd_task.h"
 #include "user_gamepad.h"
 #include "user_hogpd_config.h"
 #include "wkupct_quadec.h"
 #include "arch_console.h"
 #include "app_entry_point.h"
 #include "adc.h"
 #include "app_easy_security.h"
 #include "app_bond_db.h"
 
 struct keyboard_report_t
{
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keycode[6];
} kbd_report;
 /*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

 bool    axis_polling_on														__SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
 timer_hnd axis_update_timer_used										__SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
 
uint8_t rx_data;
uint8_t rx_buffer[100];
uint8_t rx_cnt = 0;
uint8_t rx_flag = 0;

int scan_cvt=1;
void kbd_send_ch(uint8_t ch);
void kbd_send_str(const char* text);
 /*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
// this function is called every byte received
static void uart_rx_callback(uint16_t cnt)
{
	// rx_cnt = 0;
	rx_buffer[rx_cnt] = rx_data;
	rx_cnt++;
	if(rx_data == '!')		rx_flag = 1;
	else uart_receive(UART2,&rx_data,1,UART_OP_INTR);
}


/**
 ****************************************************************************************
 * Initialize gamepad buttons
 ****************************************************************************************
 */
void user_gamepad_init(void){
	uart_register_rx_cb(UART2,uart_rx_callback);
	uart_receive(UART2,&rx_data,1,UART_OP_INTR);
	app_set_prf_srv_perm(TASK_ID_CUSTS1, SRV_PERM_UNAUTH);
	app_easy_security_bdb_init();
}

/**
 ****************************************************************************************
 * @brief Introduces a variable microsend delay for use with ADC peripheral.
 * @param[in] nof_us Number of microseconds to delay
 * @return void
 ****************************************************************************************
 */
void user_usDelay(uint32_t nof_us)
{
    while( nof_us-- ){
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }
}

/**
 ****************************************************************************************
 * Convert raw ADC reading to a 0 - 255 level value
 ****************************************************************************************
 */
uint8_t user_sample_conv(uint16_t input, uint16_t cap){
	input = (cap<input)?cap:input;
	return 0xFF&(input*255/cap);
}

/**
 ****************************************************************************************
 * Joystick location calculate and report sending for gamepad
 ****************************************************************************************
 */
#define SHIFT 0x80
const uint8_t _asciimap[128] =
{
    0x00,             // NUL
    0x00,             // SOH
    0x00,             // STX
    0x00,             // ETX
    0x00,             // EOT
    0x00,             // ENQ
    0x00,             // ACK
    0x00,             // BEL
    0x2a,           // BS   Backspace
    0x2b,           // TAB  Tab
    0x28,           // LF   Enter
    0x00,             // VT
    0x00,             // FF
    0x00,             // CR
    0x00,             // SO
    0x00,             // SI
    0x00,             // DEL
    0x00,             // DC1
    0x00,             // DC2
    0x00,             // DC3
    0x00,             // DC4
    0x00,             // NAK
    0x00,             // SYN
    0x00,             // ETB
    0x00,             // CAN
    0x00,             // EM
    0x00,             // SUB
    0x00,             // ESC
    0x00,             // FS
    0x00,             // GS
    0x00,             // RS
    0x00,             // US

    0x2c,          //  ' '
    0x1e|SHIFT,    // !
    0x34|SHIFT,    // "
    0x20|SHIFT,    // #
    0x21|SHIFT,    // $
    0x22|SHIFT,    // %
    0x24|SHIFT,    // &
    0x34,          // '
    0x26|SHIFT,    // (
    0x27|SHIFT,    // )
    0x25|SHIFT,    // *
    0x2e|SHIFT,    // +
    0x36,          // ,
    0x2d,          // -
    0x37,          // .
    0x38,          // /
    0x27,          // 0
    0x1e,          // 1
    0x1f,          // 2
    0x20,          // 3
    0x21,          // 4
    0x22,          // 5
    0x23,          // 6
    0x24,          // 7
    0x25,          // 8
    0x26,          // 9
    0x33|SHIFT,      // :
    0x33,          // ;
    0x36|SHIFT,      // <
    0x2e,          // =
    0x37|SHIFT,      // >
    0x38|SHIFT,      // ?
    0x1f|SHIFT,      // @
    0x04|SHIFT,      // A
    0x05|SHIFT,      // B
    0x06|SHIFT,      // C
    0x07|SHIFT,      // D
    0x08|SHIFT,      // E
    0x09|SHIFT,      // F
    0x0a|SHIFT,      // G
    0x0b|SHIFT,      // H
    0x0c|SHIFT,      // I
    0x0d|SHIFT,      // J
    0x0e|SHIFT,      // K
    0x0f|SHIFT,      // L
    0x10|SHIFT,      // M
    0x11|SHIFT,      // N
    0x12|SHIFT,      // O
    0x13|SHIFT,      // P
    0x14|SHIFT,      // Q
    0x15|SHIFT,      // R
    0x16|SHIFT,      // S
    0x17|SHIFT,      // T
    0x18|SHIFT,      // U
    0x19|SHIFT,      // V
    0x1a|SHIFT,      // W
    0x1b|SHIFT,      // X
    0x1c|SHIFT,      // Y
    0x1d|SHIFT,      // Z
    0x2f,          // [
    0x31,          // bslash
    0x30,          // ]
    0x23|SHIFT,    // ^
    0x2d|SHIFT,    // _
    0x35,          // `
    0x04,          // a
    0x05,          // b
    0x06,          // c
    0x07,          // d
    0x08,          // e
    0x09,          // f
    0x0a,          // g
    0x0b,          // h
    0x0c,          // i
    0x0d,          // j
    0x0e,          // k
    0x0f,          // l
    0x10,          // m
    0x11,          // n
    0x12,          // o
    0x13,          // p
    0x14,          // q
    0x15,          // r
    0x16,          // s
    0x17,          // t
    0x18,          // u
    0x19,          // v
    0x1a,          // w
    0x1b,          // x
    0x1c,          // y
    0x1d,          // z
    0x2f|SHIFT,    // {
    0x31|SHIFT,    // |
    0x30|SHIFT,    // }
    0x35|SHIFT,    // ~
    0               // DEL
};

void kbd_send_ch(uint8_t ch){
    int code;
    int i;
    if( scan_cvt ){
        if( ch > 128 ){
            ch -=128;
        }
        code = _asciimap[ch];
    }
    else
        code = ch;

    kbd_report.keycode[0]=code&0x7F;
    kbd_report.keycode[1]=0;
    if ( code & 0x80) {                  // it's a capital letter or other character reached with shift
        kbd_report.modifier |= 0x02;    // the left shift modifier
    }
    //for( i=0; i< 4;i++)
		{
			  app_hogpd_send_report(HID_GAMEPAD_AXIS_REPORT_IDX, (uint8_t*)&kbd_report, HID_GAMEPAD_AXIS_REPORT_SIZE, HOGPD_REPORT);
        //HAL_Delay(10);
		//	user_usDelay(100);
    }
    memset(kbd_report.keycode, 0 , sizeof(kbd_report.keycode));
    kbd_report.modifier = 0;
		app_hogpd_send_report(HID_GAMEPAD_AXIS_REPORT_IDX, (uint8_t*)&kbd_report, HID_GAMEPAD_AXIS_REPORT_SIZE, HOGPD_REPORT);
    //HAL_Delay(100);
		//user_usDelay(100);
}

void kbd_send_str(const char *str){
    while( *str){
        kbd_send_ch(*str);
        str++;
    }
		// do you think we have such issue when we are clinking fast?
}

void user_gamepad_update_joystick(void){

	if(rx_flag == 1){
		uart_send(UART2,rx_buffer,rx_cnt,UART_OP_INTR);
		rx_buffer[rx_cnt-1] = 0;// remove last character "!"
		kbd_send_str((char*)rx_buffer); // BLE connection and it is treated as keyboard input
		rx_cnt = 0;
		rx_flag = 0; 
		uart_receive(UART2,&rx_data,1,UART_OP_INTR); // 
	}
}

/**
 ****************************************************************************************
 * Periodically ADC value polling and joystick update callback
 ****************************************************************************************
 */
void user_gamepad_axis_polling_cb(void){
	user_gamepad_update_joystick();
	if(axis_polling_on)
		axis_update_timer_used = app_easy_timer(AXIS_UPDATE_PER, user_gamepad_axis_polling_cb);
}


/**
 ****************************************************************************************
 * Toggle the ADC reading cycle, enable the joysticks control
 ****************************************************************************************
 */
void user_gamepad_toggle_axis_polling(bool on){
	if(axis_polling_on!=on){
		if(on){
			axis_polling_on = on;
			user_gamepad_axis_polling_cb();
		}
		else{
			axis_polling_on = on;
			axis_update_timer_used = EASY_TIMER_INVALID_TIMER;
		}
	}
}

/**
 ****************************************************************************************
 * HID event handler entrance
 ****************************************************************************************
 */
void app_hid_gamepad_event_handler(ke_msg_id_t const msgid,
                                         void const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
#if BLE_HID_DEVICE
    //enum ke_msg_status_tag result;
    app_hogpd_process_handler(msgid, param, dest_id, src_id);
#endif
}
