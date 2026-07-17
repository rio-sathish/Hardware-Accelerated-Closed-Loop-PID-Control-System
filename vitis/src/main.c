#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"
// --- HARDWARE ADDRESSES ---
#define PID_BASE 0x43C00000
#define OLED_GPIO_BASE 0x41200000
#define SW_GPIO_BASE 0x41210000
#define BTN_GPIO_BASE 0x41220000
// --- OLED PINS ---
#define OLED_SCLK 0x01
#define OLED_SDIN 0x02
#define OLED_RES 0x04
#define OLED_DC 0x08
#define OLED_VDD 0x10
#define OLED_VBAT 0x20
// --- BUTTON MASKS ---
#define BTN_RIGHT 0x01
#define BTN_LEFT 0x02
#define BTN_DOWN 0x04
#define BTN_UP 0x08
#define BTN_CENTER 0x10
// --- DISPLAY MODES ---
#define MODE_NORMAL 0
#define MODE_STOP 1
#define MODE_TUNING 2
unsigned int oled_state = OLED_VDD | OLED_VBAT | OLED_RES;
//
======================================================
====
// FONT: 5x7 subset (Space, 0-9, A-Z, :, -, >, !)
// Each char = 5 bytes (columns), MSB = top pixel
//
=====================================================const unsigned char font5x7[][5] = {
{0x00,0x00,0x00,0x00,0x00}, // ' ' 0
{0x00,0x5F,0x00,0x00,0x00}, // '!' 1
{0x7C,0x12,0x11,0x12,0x7C}, // 'A' 2
{0x7F,0x49,0x49,0x49,0x36}, // 'B' 3
{0x3E,0x41,0x41,0x41,0x22}, // 'C' 4
{0x7F,0x41,0x41,0x22,0x1C}, // 'D' 5
{0x7F,0x49,0x49,0x49,0x41}, // 'E' 6
{0x7F,0x09,0x09,0x09,0x01}, // 'F' 7
{0x3E,0x41,0x49,0x49,0x7A}, // 'G' 8
{0x7F,0x08,0x08,0x08,0x7F}, // 'H' 9
{0x00,0x41,0x7F,0x41,0x00}, // 'I' 10
{0x20,0x40,0x41,0x3F,0x01}, // 'J' 11
{0x7F,0x08,0x14,0x22,0x41}, // 'K' 12
{0x7F,0x40,0x40,0x40,0x40}, // 'L' 13
{0x7F,0x02,0x0C,0x02,0x7F}, // 'M' 14
{0x7F,0x04,0x08,0x10,0x7F}, // 'N' 15
{0x3E,0x41,0x41,0x41,0x3E}, // 'O' 16
{0x7F,0x09,0x09,0x09,0x06}, // 'P' 17
{0x3E,0x41,0x51,0x21,0x5E}, // 'Q' 18
{0x7F,0x09,0x19,0x29,0x46}, // 'R' 19
{0x46,0x49,0x49,0x49,0x31}, // 'S' 20
{0x01,0x01,0x7F,0x01,0x01}, // 'T' 21
{0x3F,0x40,0x40,0x40,0x3F}, // 'U' 22
{0x1F,0x20,0x40,0x20,0x1F}, // 'V' 23
{0x3F,0x40,0x38,0x40,0x3F}, // 'W' 24
{0x63,0x14,0x08,0x14,0x63}, // 'X' 25
{0x07,0x08,0x70,0x08,0x07}, // 'Y' 26
{0x61,0x51,0x49,0x45,0x43}, // 'Z' 27
{0x3E,0x51,0x49,0x45,0x3E}, // '0' 28
{0x00,0x42,0x7F,0x40,0x00}, // '1' 29
{0x42,0x61,0x51,0x49,0x46}, // '2' 30
{0x21,0x41,0x45,0x4B,0x31}, // '3' 31
{0x18,0x14,0x12,0x7F,0x10}, // '4' 32
{0x27,0x45,0x45,0x45,0x39}, // '5' 33
{0x3C,0x4A,0x49,0x49,0x30}, // '6' 34
{0x01,0x71,0x09,0x05,0x03}, // '7' 35
{0x36,0x49,0x49,0x49,0x36}, // '8' 36
{0x06,0x49,0x49,0x29,0x1E}, // '9' 37
{0x00,0x36,0x36,0x00,0x00}, // ':' 38
{0x08,0x08,0x08,0x08,0x08}, // '-' 39
{0x00,0x41,0x22,0x14,0x08}, // '<' 40 (reused as arrow)
{0x00,0x08,0x14,0x22,0x41}, // '>' 41
{0x00,0x00,0x00,0x00,0x00}, // ' ' padding
};
// Map ASCII char to font index
int font_index(char c) {
if (c == ' ') return 0;
if (c == '!') return 1;
if (c >= 'A' && c <= 'Z') return 2 + (c - 'A');
if (c >= '0' && c <= '9') return 28 + (c - '0');
if (c == ':') return 38;
if (c == '-') return 39;if (c == '<') return 40;
if (c == '>') return 41;
return 0;
}
//
======================================================
====
// OLED LOW-LEVEL
//
======================================================
====
void oled_write(unsigned char data, int is_data) {
if (is_data) oled_state |= OLED_DC;
else oled_state &= ~OLED_DC;
Xil_Out32(OLED_GPIO_BASE, oled_state);
for (int i = 7; i >= 0; i--) {
oled_state &= ~OLED_SCLK;
if (data & (1<<i)) oled_state |= OLED_SDIN;
else oled_state &= ~OLED_SDIN;
Xil_Out32(OLED_GPIO_BASE, oled_state);
oled_state |= OLED_SCLK;
Xil_Out32(OLED_GPIO_BASE, oled_state);
} }
void oled_set_page_col(int page, int col) {
oled_write(0xB0 + page, 0);
oled_write(0x00 | (col & 0x0F), 0); // Lower nibble
oled_write(0x10 | ((col >> 4) & 0x0F), 0); // Upper nibble
}
void oled_clear() {
for (int page = 0; page < 4; page++) {
oled_set_page_col(page, 0);
for (int col = 0; col < 128; col++) oled_write(0x00, 1);
} }
void oled_fill() {
for (int page = 0; page < 4; page++) {
oled_set_page_col(page, 0);
for (int col = 0; col < 128; col++) oled_write(0xFF, 1);
} }
void oled_init() {
printf("[INIT] OLED Reset...\n");
oled_state &= ~OLED_VDD; Xil_Out32(OLED_GPIO_BASE, oled_state); usleep(1000);
oled_state &= ~OLED_RES; Xil_Out32(OLED_GPIO_BASE, oled_state); usleep(1000);
oled_state |= OLED_RES; Xil_Out32(OLED_GPIO_BASE, oled_state); usleep(1000);
oled_write(0xAE, 0); // Display offoled_write(0xD5, 0); oled_write(0x80, 0); // Clock div
oled_write(0xA8, 0); oled_write(0x1F, 0); // Mux 32 rows
oled_write(0xD3, 0); oled_write(0x00, 0); // Offset
oled_write(0x40, 0); // Start line
oled_write(0x8D, 0); oled_write(0x14, 0); // Charge pump on
oled_write(0x20, 0); oled_write(0x00, 0); // Horizontal addressing
oled_write(0xA1, 0); // Segment remap
oled_write(0xC8, 0); // COM scan dir
oled_write(0xDA, 0); oled_write(0x02, 0); // COM pins (128x32)
oled_write(0x81, 0); oled_write(0xCF, 0); // Max contrast
oled_write(0xD9, 0); oled_write(0xF1, 0); // Pre-charge
oled_write(0xDB, 0); oled_write(0x40, 0); // VCOMH
oled_write(0xA4, 0); // Resume from GDDRAM
oled_write(0xA6, 0); // Normal (not inverted)
oled_clear();
oled_write(0xAF, 0); // Display ON
oled_state &= ~OLED_VBAT; Xil_Out32(OLED_GPIO_BASE, oled_state);
usleep(100000);
printf("[INIT] OLED Ready.\n");
}
//
======================================================
====
// TEXT RENDERING
//
======================================================
====
// Draw one char at (page, col). Returns next col position.
// invert=1 → white background black text (fake "highlight")
int oled_draw_char(int page, int col, char c, int invert) {
int idx = font_index(c);
for (int i = 0; i < 5; i++) {
unsigned char byte = font5x7[idx][i];
oled_set_page_col(page, col + i);
oled_write(invert ? ~byte : byte, 1);
}
// 1px gap
oled_set_page_col(page, col + 5);
oled_write(invert ? 0xFF : 0x00, 1);
return col + 6;
}
// Draw string at (page, col). Clip at 128.
int oled_draw_string(int page, int col, const char* str, int invert) {
while (*str && col < 122) {
col = oled_draw_char(page, col, *str++, invert);
}
return col;
}
// Draw integer as string
void oled_draw_int(int page, int col, int val, int digits, int invert) {char buf[12];
int i = digits - 1;
int neg = (val < 0);
if (neg) val = -val;
buf[digits] = '\0';
while (i >= 0) {
buf[i--] = '0' + (val % 10);
val /= 10;
}
if (neg) buf[0] = '-';
oled_draw_string(page, col, buf, invert);
}
//
======================================================
====
// DISPLAY LAYOUTS
//
======================================================
====
// "Visual pattern" bar — uses checkerboard for empty region
// so it looks more structured than plain black
void oled_draw_gauge_fancy(int speed, int target, int mode) {
if (speed < 0) speed = 0;
if (speed > 255) speed = 255;
if (target < 0) target = 0;
if (target > 255) target = 255;
int bar = (speed * 120) / 255; // 0..120 px filled
int tgt_px = (target * 120) / 255; // target marker position
// --- PAGE 0: Speed bar with target marker ---
oled_set_page_col(0, 0);
for (int col = 0; col < 128; col++) {
unsigned char px;
if (col == 0 || col == 127) {
px = 0xFF; // Side border
} else if (col == tgt_px + 3) {
px = 0b10101010; // Target marker: dotted line
} else if (col - 3 <= bar) {
// Filled region: solid top, lighter bottom
px = (mode == MODE_STOP) ? 0b00011000 : 0xFF;
} else {
// Empty region: subtle dot pattern
px = (col % 4 == 0) ? 0b00010000 : 0x00;
}
oled_write(px, 1);
}
// --- PAGE 1: Thin bar bottom edge + separator ---
oled_set_page_col(1, 0);
for (int col = 0; col < 128; col++) {
unsigned char px;if (col == 0 || col == 127) {
px = 0xFF;
} else if (col - 3 <= bar) {
px = (mode == MODE_STOP) ? 0b00000011 : 0b00000111;
} else {
px = (col % 4 == 0) ? 0b00000001 : 0x00;
}
oled_write(px, 1);
}
// --- PAGE 2: Text row: "SPD:NNN TGT:NNN" ---
// Clear page first
oled_set_page_col(2, 0);
for (int col = 0; col < 128; col++) oled_write(0x00, 1);
if (mode == MODE_STOP) {
// Inverted "STOP" banner
oled_set_page_col(2, 0);
for (int col = 0; col < 128; col++) oled_write(0xFF, 1); // full white bg
oled_draw_string(2, 30, "EMERGENCY STOP", 1); // black text on white
} else if (mode == MODE_TUNING) {
oled_draw_string(2, 0, "TUNE KP:", 0);
} else {
oled_draw_string(2, 0, "SPD:", 0);
oled_draw_int(2, 26, speed, 3, 0);
oled_draw_string(2, 48, "TGT:", 0);
oled_draw_int(2, 74, target, 3, 0);
}
// --- PAGE 3: Status row ---
oled_set_page_col(3, 0);
for (int col = 0; col < 128; col++) oled_write(0x00, 1);
if (mode == MODE_TUNING) {
// Show KP value large
oled_draw_string(3, 0, "KP:", 0);
} else {
// Scrolling indicator dots (loop counter trick)
oled_draw_string(3, 0, "CRUISE CTRL", 0);
} }
// Full-screen blink for emergency stop
void oled_blink(int times) {
for (int i = 0; i < times; i++) {
oled_fill(); usleep(150000);
oled_clear(); usleep(150000);
} }
//
======================================================
====
// SAFE PID READ//
======================================================
====
int pid_compute(int kp, int ki, int kd, int err, int intg, int derv) {
Xil_Out32(PID_BASE + 0, (u32)kp);
Xil_Out32(PID_BASE + 4, (u32)ki);
Xil_Out32(PID_BASE + 8, (u32)kd);
Xil_Out32(PID_BASE + 12, (u32)err);
Xil_Out32(PID_BASE + 16, (u32)intg);
Xil_Out32(PID_BASE + 20, (u32)derv);
usleep(10);
return (int)Xil_In32(PID_BASE + 24);
}
//
======================================================
====
// BUTTON DEBOUNCE
//
======================================================
====
unsigned int btn_prev = 0;
// Returns newly pressed buttons (rising edge only)
unsigned int btn_read_edge() {
unsigned int now = Xil_In32(BTN_GPIO_BASE) & 0x1F;
unsigned int edge = now & ~btn_prev; // only newly pressed
btn_prev = now;
return edge;
}
//
======================================================
====
// MAIN
//
======================================================
====
int main() {
printf("\033[2J\033[H");
printf("================================================\n
");
printf(" CRUISE CONTROL — ARVIND PRAKASH\n");
printf(" BTN: UP/DN=KP | LR=Speed | CTR=STOP\n");
printf("================================================\n
\n");
oled_init();
// Startup animation: sweep bar
for (int s = 0; s <= 255; s += 8) {oled_draw_gauge_fancy(s, 255, MODE_NORMAL);
usleep(15000);
}
for (int s = 255; s >= 0; s -= 8) {
oled_draw_gauge_fancy(s, 0, MODE_NORMAL);
usleep(15000);
}
int kp = 3, ki = 1, kd = 2;
int current_speed = 0, target_speed = 0;
int error = 0, prev_error = 0, integral = 0, derivative = 0;
int control_effort = 0;
int mode = MODE_NORMAL;
int loop = 0;
int manual_override = 0; // Set when LEFT/RIGHT used
printf("[READY] Loop starting...\n\n");
while (1) {
loop++;
// --- READ BUTTONS (edge-triggered) ---
unsigned int btns = btn_read_edge();
// CENTER: Emergency Stop
if (btns & BTN_CENTER) {
mode = MODE_STOP;
current_speed = 0;
integral = 0;
printf("[BTN-CENTER] Emergency Stop! Loop=%d\n", loop);
oled_blink(3);
}
// UP: Increase KP
if (btns & BTN_UP) {
if (kp < 20) kp++;
mode = MODE_TUNING;
printf("[BTN-UP] KP increased to %d\n", kp);
}
// DOWN: Decrease KP
if (btns & BTN_DOWN) {
if (kp > 0) kp--;
mode = MODE_TUNING;
printf("[BTN-DOWN] KP decreased to %d\n", kp);
}
// RIGHT: Manual speed up
if (btns & BTN_RIGHT) {
target_speed += 16;
if (target_speed > 255) target_speed = 255;
manual_override = 1;
if (mode != MODE_STOP) mode = MODE_NORMAL;
printf("[BTN-RIGHT] Target speed -> %d\n", target_speed);}
// LEFT: Manual speed down
if (btns & BTN_LEFT) {
target_speed -= 16;
if (target_speed < 0) target_speed = 0;
manual_override = 1;
if (mode != MODE_STOP) mode = MODE_NORMAL;
printf("[BTN-LEFT] Target speed -> %d\n", target_speed);
}
// If not manually overriding, read switches for target
if (!manual_override) {
target_speed = (int)(Xil_In32(SW_GPIO_BASE) & 0xFF);
}
// Resume from STOP after CENTER press + any other button
if (mode == MODE_STOP && (btns & ~BTN_CENTER)) {
mode = MODE_NORMAL;
printf("[RESUME] Exiting stop mode.\n");
}
// Auto-exit tuning mode after no button activity (show normal)
if (mode == MODE_TUNING && btns == 0) {
// keep tuning display for 5 loops then auto-revert
static int tune_timer = 0;
tune_timer++;
if (tune_timer > 5) { mode = MODE_NORMAL; tune_timer = 0; }
}
// --- PID (skip if stopped) ---
if (mode != MODE_STOP) {
error = target_speed - current_speed;
integral += error;
if (integral > 200) integral = 200;
if (integral < -200) integral = -200;
derivative = error - prev_error;
control_effort = pid_compute(kp, ki, kd, error, integral, derivative);
current_speed += (control_effort / 16) - 4;
if (current_speed < 0) current_speed = 0;
if (current_speed > 255) current_speed = 255;
}
prev_error = error;
// --- DISPLAY ---
oled_draw_gauge_fancy(current_speed, target_speed, mode);
// --- UART LOG ---
printf("[%04d] SPD:%3d TGT:%3d EFFORT:%6d KP:%d MODE:%d\n",
loop, current_speed, target_speed, control_effort, kp, mode);
usleep(2000000);}return 0;
}