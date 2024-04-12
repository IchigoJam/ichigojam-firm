// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __BASIC_H__
#define __BASIC_H__

#include "lang.h"

#ifdef IJB_USE_EXCEPTION
#define IJB_ERR_CHK()
#define IJB_ERR_CHK1(n)
#define IJB_ERR_CHKB(n)
#define IJB_ERR_RETURN1(x)
#define IJB_ERR_RETURN()
#else
#define IJB_ERR_CHK() if (_g.err) return;
#define IJB_ERR_CHK1(n) if (_g.err) return 0;
#define IJB_ERR_CHKB(n) if (_g.err) break;
#define IJB_ERR_RETURN1(x) return (x)
#define IJB_ERR_RETURN() return
#endif

#define IJB_SIZEOF_LIST (1024 + 2) // uint8

#define IJB_SIZEOF_VAR 26 // uint16 A-Z
#define IJB_SIZEOF_ARRAY 102 // uint16 0.9.9-rc6 (101 -> 102)
#define IJB_SIZEOF_GOSUB_STACK 30 // call stack size for GOSUB/RETURN // 1.2b13 10->30  16*4=64byte -> 36*2=72 (+8byte)
#define IJB_SIZEOF_FOR_STACK 6 // call stack size for FOR/NEXT // 1.2b10 4->6

#ifdef BOOT_CHECK
extern int bootflg;
#endif

//void wait(int n);
//void _printf(const char* fmt, ...);
STATIC void put_chr(char c);
STATIC void put_str(const char* s);

S_INLINE int stopExecute();

static int basic_execute(char* commandline);
S_INLINE void basic_init();
S_INLINE int basic_listSize();

static int IJB_save(int n, uint8* list, int size); // 0:ok
static int IJB_load(int n, uint8* list, int sizelimit, int init); // ret:size if:-1 _g.err
static int i2c0_init(); /* Initialize I2C module  0:ok 1:_g.err */
S_INLINE int IJB_file();

static int IJB_wait(int n, int active);
//S_INLINE int IJB_waitLittle(int n);
S_INLINE void IJB_led(int on);
/* S_INLINE */ int IJB_random(int n);
S_INLINE void IJB_random_seed(int n);
//void IJB_motor(int ch, int n);
/*S_INLINE*/ int IJB_in();
S_INLINE int IJB_ana(int n);
/*S_INLINE*/ void IJB_out(int port, int st);
/*S_INLINE*/ void IJB_pwm(int port, int plen, int len);
/*S_INLINE*/ int IJB_btn(int n); // -4byte
S_INLINE void IJB_clo();
static void IJB_sleep();
S_INLINE void IJB_reset();
static void IJB_input(char** line);
S_INLINE void IJB_uart(int16 txd, int16 rxd);
S_INLINE void IJB_bps(int uart, int i2c);
S_INLINE int IJB_i2c(uint8 writemode, uint16* param); // -1:ad error 1:ok 0:i2c error

S_INLINE int IJB_peek(int ad);
S_INLINE void IJB_poke(int ad, int n);
S_INLINE int IJB_usr(int ad, int n);
S_INLINE void IJB_lcd(uint mode); // 1.2b32
#ifndef NO_KBD_COMMAND
S_INLINE void IJB_kbd(uint mode); // 1.5b1
#ifdef VERSION15
S_INLINE void IJB_dac(int port, int val); // 1.5
#endif
#endif

// keyboard
int key_getKey(); // from keybuffer, ret 0 if nul -> ret -1 if null 1.2b19
static void key_clearKey();
S_INLINE uint key_getKeyboardID();

// psg
S_INLINE void psg_playMML(char* mml);
S_INLINE void psg_beep(int tone, int len);
S_INLINE void psg_tempo(int tempo);
S_INLINE int psg_sound();

// screen
static void screen_clear();
static void video_on();
static void video_off(int clkdiv);
static short video_tick(int n);
S_INLINE void video_clt();
static void screen_clp();
static uint8 screen_get(int x, int y);
static uint8 screen_getCurrent();
static void screen_locate(int x, int y);
static void screen_scroll(int dir);
static void screen_line(int x1, int y1, int x2, int y2, int cmd); // 1.3b8

#ifdef EXT_IOT

// ?IOT.IN()
// IOT.OUT 8
static int16 iot_in();
static void iot_out(int n);
/*
#define iot_in() iot_inout(1, 0)
#define iot_out(n) iot_inout(0, (n))
static int16 iot_inout(int typein, int n);
*/
static void iot_out2(int ad, int len, int flash);

#endif

S_INLINE void ws_out(int port, int nled, int reapeat);

// util -----------------------------------------------------

//#define clearMemory(p, len) memset(p, 0, len)


S_INLINE uint8 basic_toupper(uint8 c) {
	if (c >= 'a' && c <= 'z') {
		return c & 0b1011111; // 速度一緒 なぜか小文字のほうが速い
//		return c + ('A' - 'a');
	}
	return c;
}
uint strlen8(const char* s) {
	pint n = (pint)s;
	while (*s++);
	return (int)((pint)s - n - 1);
}
/*STATIC*/ int put_num(int n) { // for RISC-V
	int len = 0;
	if (n < 0) {
		put_chr('-');
		len++;
		n = -n;
	}
	uint v = 0;
	for (uint d = 10000; d > 0; d /= 10) {
		//uint c = (uint)n / d;
		uint c = calcDivU(n, d);
		v |= c;
		if (v || d == 1) {
			put_chr(c + '0');
			len++;
		}
		n -= c * d;
	}
	return len;
}
//uint8 getBeam(int16 n) {
static int getBeam(int n) {
	int res = 1;
	if (n < 0) {
		res++;
		n = -n;
	}
	int chk = 10;
	while (n >= chk) {
		res++;
		chk *= 10;
	}
	return res;
}
STATIC void put_str(const char* s) {
	while (*s)
		put_chr(*s++);
}
static void put_strmem(int n, int m) {
	if (n >= OFFSET_RAMROM) {
		char* s = (char*)(ram + n - OFFSET_RAMROM);
		for (;;) {
			if (*s == '"' || *s == '\0' || m == 0 || s >= (char*)ram + SIZE_RAM)
				break;
			put_chr(*s);
			s++;
			m--;
		}
	}
}


extern /*INLINE*/ int sin360(int deg); // +168byte, APIから使えるようにして +4byte
/*
// -- sin/cos/tan from @yrm
int32 sin32(int32 s);
S_INLINE int32 cos32(int32 s);
//S_INLINE int32 tan32(int32 s);
int atan2(int y, int x);
*/

// virtual machine --------------------------------------------------------

#include "ram.h"

//char list[IJB_SIZEOF_LIST] __attribute__ ((aligned(4))); // プログラムリスト 文字のまま格納 4byte align 必要 to write flash
char* const list = (char*)ram + OFFSET_RAM_LIST;
//		{
//			int16 linenum;			行番号 1-32767  行番号=0 プログラムの末尾、末尾も兼ねる
//			uint8 n;				ステートメントのバイト数 0-254 // 必ず 2の倍数
//			char statement[n];		ステートメント
//			(char dummy)			for 2 byte align
//			uint8 termination = 0;	end flg
//		}*

// listの後にLINEBUFをとれば、下記、intじゃなくてint16で済む 14*2byteの節約可能、LINE_BUFをLISTの後にとる必要あり
#define POS_RAM 0x10000000 // (1<<56)
//char* gosubstack[IJB_SIZEOF_GOSUB_STACK];
//char* forstack[IJB_SIZEOF_FOR_STACK];

#ifdef MEM_UNDER64KB
#define pchar uint16 // RAMが64kbyte以下の時専用、RAM節約できる
#else
#define pchar char* // 通常
#endif

pchar gosubstack[IJB_SIZEOF_GOSUB_STACK];
pchar forstack[IJB_SIZEOF_FOR_STACK];

typedef struct {
	uint16 code; // 順番入れ替えるとサイズ増える // uint8 -> uint16 小さくなる
	int16 value;
} Token;

char* pc;		// program counter
//char* pcnext;   // next line // alignナシ版
char* pcbreak;	// break point
char* lasttoken; // 取得したトークンの先頭 for token_back

#define TOKEN_CACHE // 212byte使う、40%高速化する

#ifdef TOKEN_CACHE
char* lasttokenpc; // 取得したトークンの最後 for token_back
Token bklasttoken;
#endif

//int16 var[IJB_SIZEOF_ARRAY + IJB_SIZEOF_VAR];		// 変数 [0]-[IJB_SIZEOF_ARRAY-1], A-Z
int16* const var = (int16*)(ram + OFFSET_RAM_VAR);


/*
中間コード格納式
uint8 tokenid
if (tokenid == STRING || tokenid == REM) {
	uint8 len
	char* s[len] // null終端ではない
} else if (tokenid == NUMBER) {
	int16 number
}
*/

S_INLINE int16 list_getNumber(uint16 index) {
	return *(int16*)(list + index);
}
S_INLINE uint8 list_getLength(uint16 index) {
	return *(uint8*)(list + index + 2);
}
S_INLINE void list_setNumber(uint16 index, int16 num) {
	*(int16*)(list + index) = num;
}
void list_setLength(uint16 index, uint8 num) {
	*(uint8*)(list + index + 2) = num + (num & 1);
}
static int16 list_find(int16 number) {
	int index = 0;
	for (;;) {
		int n = list_getNumber(index);
		if (n == 0 || n >= number)
			break;
		index += list_getLength(index) + 4;
	}
	return index;
}
static int16 list_findGoto(int16 number) {
	int index = list_find(number);
	if (list_getNumber(index) != number)
		index = -1;
	return index;
}
S_INLINE void list_setPC(int n) {
	pc = list + n + 3;
	/*
	pcnext = pc + *(pc - 1) + 4;
	if (pcnext >= list + _g.listsize) {
		pcnext = 0;
	}
	*/
}


// error -----------------------------------------------------------------

// -> error.h

/*
//#define USE_ILLEGAL_ARGUMENT_ERROR // 28byte節約
//#undef USE_ILLEGAL_ARGUMENT_ERROR // -28byte
#ifndef USE_ILLEGAL_ARGUMENT_ERROR
#define ERR_ILLEGAL_ARGUMENT ERR_SYNTAX_ERROR // SYNTAX_ERRORを代わりに使う
#endif
*/


// token -------------------------------------------------------


#ifdef EXT_SENSEHAT
#include <stdio.h>
#endif


// プログラムをプリインするテスト
//static const int var600[256] __attribute__ ((section(".ichigojam_data"))) = { 0xff, 0xff,3,3,3,9,8,7,0,8,8,0,3 };



#define N_TOKEN_EXPRESSION TOKEN_LET
#define N_TOKEN_OFFSET 4

#ifdef VERSION15
#include "tokens_v1.5.h"
#else
#include "tokens_v1.4.h"
#endif

// command ------------------------------------------------------------------

static void command_error(int erridx); // no return -> HardFault
static void command_rem();
static void command_let(char token);
static int command_let2(int var);
S_INLINE void command_edit(int number);
S_INLINE void command_clv();
static void command_if(); // S_INLINE化すると 4byte増
static void command_for(); // S_INLINEで26byte増
static void command_next();
S_INLINE void command_goto();
S_INLINE void command_gosub();
S_INLINE void command_return();
S_INLINE void command_cont();
static void command_print(); // 元はS_INLINEではなかった
S_INLINE void command_input();
S_INLINE void command_new();
S_INLINE void command_list();
S_INLINE void command_run();
S_INLINE void command_end();
static void command_load(int mode);
S_INLINE void command_save();
S_INLINE void command_files();
S_INLINE void command_led();
S_INLINE void command_out();
S_INLINE void command_pwm();
S_INLINE void command_clo();
S_INLINE void command_wait();
//S_INLINE void command_lwait(); // ver 1.2
S_INLINE void command_cls();
S_INLINE void command_locate();
//S_INLINE void command_motor();
static void command_renum(); // S_INLINEで4byte増える
S_INLINE void command_sleep(); // S_INLINEで8byte増
S_INLINE void command_reset(); // S_INLINEで増
S_INLINE void command_bps();
S_INLINE void command_clt();
S_INLINE void command_play();
S_INLINE void command_beep(); // S_INLINEで4増
S_INLINE void command_tempo();
S_INLINE void command_video();
S_INLINE void command_scroll();
S_INLINE void command_clp();
S_INLINE void command_poke();
S_INLINE void command_copy();
S_INLINE void command_clk();
S_INLINE void command_help();
S_INLINE void command_uart();
S_INLINE void command_srnd();
S_INLINE void command_draw();
S_INLINE void command_at();
S_INLINE void command_ok();

S_INLINE void command_lcd();
//#ifndef NO_KBD_COMMAND
#ifdef VERSION15
S_INLINE void command_kbd();
S_INLINE void command_dac();
#endif
#ifdef EXT_SENSEHAT
S_INLINE void command_sh_mlsp();
#endif

#ifdef EXT_IOT
S_INLINE void command_iot_out();
#endif
S_INLINE void command_ws_out(int port);


// token -----------------------------------------------------------------

static uint8 token_getChar();
//S_INLINE uint8 token_getCharWithSpace(); // 1.1b12 - 1.2b52廃止
#ifdef USE_VOID_TOKEN_GET
static void token_get(Token *);
#define Token_get(x) token_get(&(x))
S_INLINE int token_getCode() { Token t; Token_get(t); return t.code; }
#else
static Token token_get();
#define Token_get(x) { (x) = token_get(); }
#define token_getCode(x) token_get().code
#endif
static void token_back(); // S_INLINE -> size up
static int16 token_getArrayIndex();
static void token_end();
S_INLINE void token_puts();
static char* token_skipstr();
static int16 token_expression();
static int16 token_expression1();
static int16 token_expression2();
static int16 token_expression3();
static int16 token_expression4();
static int16 token_expression5();

// extension -------------

#ifdef USE_EXTENSION
#include "extension.h"
#endif

// basic ----------------------------------------------------------------

static void basic_clearVars() {
	memclear((uint8*)var, (IJB_SIZEOF_ARRAY + IJB_SIZEOF_VAR) * 2);
}
S_INLINE void basic_init() {
	basic_clearVars();
	memclear4((uint8*)list, IJB_SIZEOF_LIST);
	pc = pcbreak = NULL;

	#ifdef USE_EXTENSION
	extension_init();
	#endif
}
S_INLINE int basic_listSize() {
	return _g.listsize;
}

#define BASIC_RESULT_STOP_OR_ERR 0
#define BASIC_RESULT_EXECUTE 1 // no error
#define BASIC_RESULT_EDIT 2
//#define BASIC_RESULT_ERR 3
#ifdef IJB_DONT_LOOP
#define BASIC_RESULT_CONTINUE 4
#define BASIC_RESULT_INPUT 5
#define BASIC_RESULT_LIST 6
#define BASIC_RESULT_FILES 7
#endif

// ret: 0 stop or err, 1 execute, 2 edit, 4 continue(ifdef IJB_DONT_LOOP), 5 input(ifdef IJB_DONT_LOOP)
int basic_execute(char* commandline) {
#ifdef IJB_DONT_LOOP
	if (commandline) {
#endif

	_g.err = 0;
	_g.ngosubstack = 0;
	_g.nforstack = 0;
	_g.tokenmode = 0;
	pc = commandline;
	// pcnext = 0;
	lasttoken = 0;
#ifdef IJB_DONT_LOOP
	}
#endif

	for (;;) {
		if (pc == NULL)
			break;
		token_getChar();
		if (*pc == ':') {
			pc++;
			continue;
		} else if (*pc == '\'') {
			command_rem();
			continue;
		} else if (*pc == '\0') {
			if (((pint)pc & 1) == 0) { // align 2byte
				pc++;
			}
			if (pc >= list && pc + 4 < list + _g.listsize) {
				pc += 4;
				continue;
			}
			break;
		}

		Token token; Token_get(token);
		/*
		put_chr('*');
		put_num(token.code);
		put_chr('\n');
		*/
//		printf("token:%d\n", token.code);
		switch (token.code) {
			case TOKEN_NULL:											break;
			case TOKEN_NUMBER:		command_edit(token.value); pc = NULL;	return BASIC_RESULT_EDIT;
			case TOKEN_VAR:
			case TOKEN_ARRAY:		token_back(); command_let(TOKEN_EQ);	break;
			case TOKEN_AT:			command_at();							break;
			case TOKEN_IF:			command_if();							break;
			case TOKEN_ELSE:		command_rem();							break;
			case TOKEN_FOR:			command_for();							break;
			case TOKEN_NEXT:		command_next();							break;
			case TOKEN_GOTO:		command_goto();							break;
			case TOKEN_GOSUB_1:
			case TOKEN_GOSUB_2:		command_gosub();						break;
			case TOKEN_RETURN_1:
			case TOKEN_RETURN_2:	command_return();						break;
			case TOKEN_END:			command_end();							break;
			case TOKEN_REM_1:
			case TOKEN_REM_2:		command_rem();							break;
			case TOKEN_CONT:		command_cont();							break;
			//52
			//53
//			case TOKEN_OK:			command_ok();							return 2; // 1.2b62まで
			case TOKEN_OK:			command_ok();							break; // 1.2b63
			case TOKEN_NEW:			command_new();							break;


			case TOKEN_RUN:			command_run();							break;
			case TOKEN_LOAD:	
			case TOKEN_LRUN:		command_load(token.code);				break;
			case TOKEN_SAVE:		command_save();							break;
			case TOKEN_CLV_1:
			case TOKEN_CLV_2:		command_clv();							break;
			case TOKEN_LET:			command_let(TOKEN_COMMA);				break;
			case TOKEN_CLS:			command_cls();							break;
			case TOKEN_LOCATE_1:
			case TOKEN_LOCATE_2:	command_locate();						break;
			case TOKEN_PRINT_1:
			case TOKEN_PRINT_2:		command_print();						break;
			case TOKEN_INPUT:		command_input();						break;
			//68
			case TOKEN_CLK:		command_clk();							break;
			case TOKEN_SRND:	command_srnd();							break;
			case TOKEN_DRAW:	command_draw();							break; // 1.38b8

			//80
			case TOKEN_WAIT:	command_wait();							break;
			case TOKEN_CLT:		command_clt();							break;
			case TOKEN_OUT:		command_out();							break;
			case TOKEN_PWM:		command_pwm();							break;
			case TOKEN_LED:		command_led();							break;
			//90

			//93
			case TOKEN_CLO:		command_clo();							break;
			case TOKEN_FILES:	command_files();						break;
			//100


			case TOKEN_SLEEP:	command_sleep();						break;
			case TOKEN_RESET:	command_reset();						return BASIC_RESULT_EDIT; // break; // for wasm 容量増加?

			case TOKEN_RENUM:	command_renum();						break;

			//68
			case TOKEN_SCROLL:	command_scroll();						break;
			case TOKEN_VIDEO:	command_video();						break;
			//76

			case TOKEN_BPS:		command_bps();							break;
			case TOKEN_BEEP:	command_beep();							break;
			case TOKEN_TEMPO:	command_tempo();						break;
			case TOKEN_PLAY:	command_play();							break;

			case TOKEN_POKE:	command_poke();							break;
			case TOKEN_COPY:	command_copy();							break;
			case TOKEN_CLP:		command_clp();							break;
			case TOKEN_UART:	command_uart();							break;
			case TOKEN_LCD:		command_lcd();							break;
			case TOKEN_HELP:	command_help();							break;
			case TOKEN_LIST:	command_list();							break;
//#ifndef NO_KBD_COMMAND
#ifdef VERSION15
			case TOKEN_KBD:		command_kbd();							break;
			case TOKEN_DAC:		command_dac();							break;
#endif
//			case TOKEN_MOTOR:	command_motor();						break;
//			case TOKEN_PSET:	command_copy();							break; // 3argments をまとめると入る?
#ifdef EXT_SENSEHAT
			case TOKEN_SH_MLSP:	command_sh_mlsp();							break;
#endif
#ifdef EXT_IOT
			case TOKEN_IOT_OUT:	command_iot_out();							break;
#endif
			//case TOKEN_WS_OUT:	command_ws_out(1);							break;
			case TOKEN_WS_LED:	command_ws_out(1);							break; // 7 == LED
			default:
				#ifdef USE_EXTENSION
				if (extension_command(token.code)) {
					break;
				}
				#endif
				command_error(ERR_SYNTAX_ERROR);
				break;
		}
//		xprintf("_g.err %d\n", _g.err);
		if (_g.err)
			return BASIC_RESULT_STOP_OR_ERR;
		if (stopExecute()) {
			command_error(ERR_BREAK);
//			IJB_out(outbuf = 0);
//			IJB_led(0);
			return BASIC_RESULT_STOP_OR_ERR; //IJB_ERR_RETURN1(BASIC_RESULT_STOP_OR_ERR); // 1.2.1 BASIC_RESULT_STOP -> BASIC_RESULT_ERR -> 1.3b4 BASIC_RESULT_STOP_OR_ERR
		}
#ifdef IJB_DONT_LOOP
		// input or list or continue
		switch (token.code) {
			case TOKEN_INPUT:
				return BASIC_RESULT_INPUT;
			case TOKEN_LIST:
				return BASIC_RESULT_LIST;
			case TOKEN_FILES:
				return BASIC_RESULT_FILES;
		 }
		 return BASIC_RESULT_CONTINUE;
#endif
	}
	return BASIC_RESULT_EXECUTE;
}

S_INLINE void command_edit(int number) {
	if (number <= 0 || (pc >= list && pc < list + IJB_SIZEOF_LIST)) { // プログラム実行時は不可
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int foundindex = list_find(number);
	if (list_getNumber(foundindex) == number) { // delete
		const uint len = list_getLength(foundindex) + 4;
		uint16 dst = foundindex;
		uint16 src = foundindex + len;
		while (src < _g.listsize) {
			list[dst++] = list[src++];
		}
		_g.listsize -= len;
		list_setNumber(_g.listsize, 0);
	}

	if (*(pc - 1) == ' ') {
		for (;;) {
			if (*(pc - 2) == ' ') {
				pc--;
			} else {
				break;
			}
		}
	}
	if (*pc != 0) {
		const uint len = strlen8(pc);
		const uint align = len & 1; // align 2byte
		uint16 src = _g.listsize;
		uint16 dst = _g.listsize + len + align + 4;
		if (dst + 2 > IJB_SIZEOF_LIST) {
			command_error(ERR_OUT_OF_MEMORY);
			return; // IJB_ERR_RETURN();
		}
		_g.listsize = dst;
		while (src > foundindex) {
			list[--dst] = list[--src];
		}
		list_setNumber(_g.listsize, 0);

		list_setNumber(foundindex, number);
		list_setLength(foundindex, len);
		dst = foundindex + 3;

		for (;;) {
			char c = *(pc++);
			list[dst++] = c;
			if (c == '\0')
				break;
		}
		if (align) {
			list[dst] = '\0';
		}
	}
}
void basic_printError() {
	if (noresmode) // 1.2b50
		return;

	#ifndef IJB_USE_EXCEPTION // 1.4b14 for web/ap
	if (_g.cursory == -1)
		_g.cursory = 0;
	#endif
	
	//put_str(ERR_MESSAGES[_g.err - 1]);
	put_str(ERR_MESSAGES + _g.err);

//	_printf("line0 %x %x\n", pc, list);
	if (pc >= list) {
		int index = 0;
		for (;;) {
			int n = list_getNumber(index);
			if (n == 0) // ver 1.2beta5 fixed
				break;
			int size = list_getLength(index);
//			_printf("line %n %x %x %x\n", n, pc, list, list + index + size + 4);
			int dpc = list + index + size + 4 - pc;
//			if (pc < list + index + size + 4) {
			if (dpc >= 0) {
				put_str(" in ");
				put_num(n);
				put_chr('\n');

				// エラー行表示
				put_num(list_getNumber(index));
				put_chr(' ');
				put_str(list + index + 3);

				// エラー位置へカーソル移動?
//	put_chr('\n');
//				put_num(dpc);

				pcbreak = pc;
				break;
			}
			index += size + 4;
		}
	}
	put_chr('\n');
	psg_beep(10, 3); // ver 1.0.2b4
}
void command_error(int erridx) {
	if (errorignore) //  || noresmode) 1.2b50でcomment out
		return;
	_g.err = erridx;

	#ifdef IJB_USE_EXCEPTION
	*(char*)0 = 0;
	#else
	basic_printError();
	#endif
}

// token ----------------------------------------------------------------------------

static uint8 token_getChar() {
	for (;;) {
		uint8 c = *pc;
		if (c != ' ') { // (c == '\0' || c > ' ')
			if (c == ':') // (c == ':' || c == '\'')
				return 0;
			return basic_toupper(c);
		}
		pc++;
	}
}
/* // 1.1b12 で導入、1.2b52 で廃止
S_INLINE uint8 token_getCharWithSpace() { // 60byte増
	uint8 c = *pc;
	if (c == ':') // || c == '\'')
		return 0;
	return _toupper(c);
}
*/
//#include <stdio.h>

#ifdef USE_VOID_TOKEN_GET
#define Token_code token->code
#define Token_value token->value
#define Token_object(x) (*(x))
#define Token_return(x) return
static void token_get(Token *token)
#else
#define Token_code token.code
#define Token_value token.value
#define Token_object(x) (x)
#define Token_return(x) return (x)
static Token token_get()
#endif
{
#ifdef TOKEN_CACHE
	if (pc == lasttoken && lasttokenpc) {
		pc = lasttokenpc;
//	printf("%d %d %d %d hit\n", (int)pc, (int)lasttoken, (int)lasttokenpc, bklasttoken.code);
#ifdef USE_VOID_TOKEN_GET
		*token = bklasttoken;
#endif
		Token_return(bklasttoken);
	}
//	printf("%d %d %d\n", (int)pc, (int)lasttoken, (int)lasttokenpc);
#endif
	
#ifdef USE_VOID_TOKEN_GET
	token->code = 0;
	token->value = 0;
#else
	Token token = { 0, 0 };
#endif
	uint8 c = token_getChar();

	lasttoken = pc;
	if (c == '\0') {
		Token_code = TOKEN_NULL;
	} else if (c >= '0' && c <= '9') { // number
		Token_code = TOKEN_NUMBER;
//		Token_value = 0;
		for (;;) {
			Token_value = Token_value * 10 + (c - '0');
			pc++;
			c = token_getChar();
			if (c < '0' || c > '9')
				break;
		}
	} else if (c == '#') { // hex number
		pc++;
		c = token_getChar();
		if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
			Token_code = TOKEN_ERROR;
		} else {
			Token_code = TOKEN_NUMBER;
			int value = 0;
			for (;;) {
				if (c <= '9')
					c -= '0';
				else
					c -= 'A' - 10;
//				Token_value = (Token_value << 4) + c;
				value = (value << 4) + c;
				pc++;
				c = token_getChar(); // 1.1b12 -> 空白で切ることに -> 1.2b52 戻す
				//				c = token_getCharWithSpace(); // 1.1b12 -> 空白で切ることに
				if (c == 'L' || c == 'N') { // 16進数後のELSE対応 // 1.2b62 // 16進数後のAND対応 1.4b04
					pc--;
					value = value >> 4;
					break;
				}
				if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')))
					break;
			}
			Token_value = value;
		}
	} else if (c == '`') { // bin number
		pc++;
		c = token_getChar();
		if (c != '0' && c != '1') {
			Token_code = TOKEN_ERROR;
		} else {
			Token_code = TOKEN_NUMBER;
//			Token_value = 0;
			for (;;) {
				Token_value = (Token_value << 1) + (c - '0');
				pc++;
				c = token_getChar();
				if (c != '0' && c != '1') {
					break;
				}
			}
		}
	} else { // token
		int max = N_TOKEN;
		if (_g.tokenmode) {
			max = N_TOKEN_EXPRESSION; // 1.2b64
		}
		const uint8* ptoken = TOKENS;
		uint8 c2 = 0;
		for (int i = 0; i < max; i++) {
			uint8 len = *ptoken;
			int hit = 1;
			for (int j = 1; j < len; j++) {
				// 高速化のためにinline展開(token_getCharの一部)
				for (;;) {
					c2 = *pc;
					pc++;
					if (c2 != ' ') {
						break;
					}
				}
				if (basic_toupper(c2) != ptoken[j]) {
					hit = 0;
					break;
				}

//				if (token_getChar() != c) { // 1.2b52 戻してみる?
//				if (token_getCharWithSpace() != c) { // 1.1b12 (hexとの混同防ぐために空白で切っていた)
//					hit = 0;
//					break;
//				}
//				pc++;
			}
			if (hit) {
				Token_code = i + N_TOKEN_OFFSET;
//				Token_return(token);
				goto RET;
			}
			ptoken += len;
			pc = lasttoken;
		}
		// var
//		pc = lasttoken;
//		c = token_getChar();
		#ifdef USE_EXTENSION
		int n = extension_parse();
		if (n) {
			Token_code = n;
			goto RET;
		}
		#endif

		if (c >= 'A' && c <= 'Z') {
			pc++;
			Token_code = TOKEN_VAR;
			Token_value = c - ('A' - IJB_SIZEOF_ARRAY);
		} else {
			pc++; // 1.4b10
			Token_code = TOKEN_ERROR;
		}
	}
RET:
#ifdef TOKEN_CACHE
	bklasttoken = Token_object(token);
//	printf("%c %d %d push\n", *lasttoken, (int)pc, bklasttoken.code);
	lasttokenpc = pc;
#endif
	Token_return(token);
}
static void token_back() {
	pc = lasttoken;
}
static int16 token_getArrayIndex() {
	int16 v = token_expression();
	IJB_ERR_CHK1(0);

	Token t; Token_get(t);
	if (t.code != TOKEN_ARRAY_E) {
		command_error(ERR_SYNTAX_ERROR);
		return 0; // IJB_ERR_RETURN1(0);
	}
	if (v < 0 || v >= IJB_SIZEOF_ARRAY) {
		command_error(ERR_INDEX_OUT_OF_RANGE);
		return 0; // IJB_ERR_RETURN1(0);
	}
	return v;
}
static void token_end() {
	int code = token_getCode(); // 容量削減? 1.2b62
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		command_error(ERR_SYNTAX_ERROR);
	}
	/*
	char c = token_getChar();
	if (c == 'E') {
		pc++;
		c = token_getChar();
		if (c != 'L') {
			command_error(ERR_SYNTAX_ERROR);
			return;
		}
		pc++;
		c = token_getChar();
		if (c != 'S') {
			command_error(ERR_SYNTAX_ERROR);
			return;
		}
		pc++;
		c = token_getChar();
		if (c != 'E') {
			command_error(ERR_SYNTAX_ERROR);
			return;
		}
		command_rem();
		return;
	} else if (c != 0) {
		command_error(ERR_SYNTAX_ERROR);
	}
	*/
}
S_INLINE void token_puts() {
//	if (*pc == '"') // 1.2beta9
//		pc++;
	while (*pc && *pc != '"') {
		put_chr(*(pc++));
	}
	if (*pc == '"')
		pc++;
}
static char* token_skipstr() {
//	if (*pc == '"') // 1.2b9
//		pc++;
	char* res = pc;
	while (*pc && *pc != '"') {
		pc++;
	}
	if (*pc == '"')
		pc++;
	return res;
}

// token expression ---------------------------------------------
//extern unsigned int _ebss;
//#include "LPC1100.h"

/*
int16 token_expression() {
	//	_printf("t MSP   :%x %x\n", __get_MSP(), &_ebss);
	int st = 0;
	for (;;) {
		switch (st) {
		  case 0: {
				push(1)
				int16 value = token_expression0();
				if (_g.err)
					return value;
				for (;;) {
					Token t; Token_get(t);
					if (t.code != TOKEN_LOR) {
						token_back();
						break;
					}
					int16 value2 = token_expression0();
					if (_g.err)
						break;
					value = value || value2;
				}
				return value;
			}
}
int16 token_expression0() {
	int16 value = token_expression1();
	if (_g.err)
		return value;
	for (;;) {
		Token t; Token_get(t);
		if (t.code != TOKEN_LAND) {
			token_back();
			break;
		}
		int16 value2 = token_expression1();
		if (_g.err)
			break;
		value = value && value2;
	}
	return value;
}
*/

static int16 token_expression() {
	_g.tokenmode = 1; // 式モードに切り替え
#ifdef TOKEN_CACHE
	lasttokenpc = 0;
#endif
	int16 value = token_expression1();
	if (!_g.err) {
		for (;;) {
			Token t; Token_get(t);
			if (t.code != TOKEN_LOR_1 && t.code != TOKEN_LOR_2) {
				token_back();
				break;
			}
			int16 value2 = token_expression1();
			IJB_ERR_CHKB();
			value = value || value2;
		}
	}
	_g.tokenmode = 0;
#ifdef TOKEN_CACHE
	lasttokenpc = 0;
#endif
	return value;
}
static int16 token_expression1() {
	int16 value = token_expression2();
	IJB_ERR_CHK1(value);
	for (;;) {
		Token t; Token_get(t);
		if (t.code != TOKEN_LAND_1 && t.code != TOKEN_LAND_2) {
			token_back();
			break;
		}
		int16 value2 = token_expression2();
		IJB_ERR_CHKB();
		value = value && value2;
	}
	return value;
}
static int16 token_expression2() {
	int16 value = token_expression3();
	IJB_ERR_CHK1(value);
	for (;;) {
		Token t; Token_get(t);
		if (t.code < TOKEN_EQEQ || t.code > TOKEN_GT) {
			token_back();
			break;
		}
		int16 rv = token_expression3();
		IJB_ERR_CHKB();
		switch (t.code) {
			case TOKEN_GT:		value = value > rv;		break;
			case TOKEN_EQEQ:
			case TOKEN_EQ:		value = value == rv;	break;
			case TOKEN_GE:		value = value >= rv;	break;
			case TOKEN_LT:		value = value < rv;		break;
			case TOKEN_NE_1:
			case TOKEN_NE_2:
			case TOKEN_NE_3:	value = value != rv;	break;
			case TOKEN_LE:		value = value <= rv;	break;
		}
	}
	return value;
}
static int16 token_expression3() {
	int16 value = token_expression4();
	IJB_ERR_CHK1(value);
	for (;;) {
		Token t; Token_get(t);
		if (t.code < TOKEN_PLUS || t.code > TOKEN_OR) {
			token_back();
			break;
		}
		int16 value2 = token_expression4();
		IJB_ERR_CHKB();
		/*
		switch (t.code) {
			case TOKEN_PLUS:	value += value2;	break;
			case TOKEN_MINUS:	value -= value2;	break;
			case TOKEN_OR:		value |= value2;	break;
		}
		*/
		if (t.code == TOKEN_PLUS) {
			value += value2;
		} else if (t.code == TOKEN_MINUS) {
			value -= value2;
		} else { // must be TOKEN_OR
			value |= value2;
		}
	}
	return value;
}
static int16 token_expression4() {
	int16 value = token_expression5();
	IJB_ERR_CHK1(value);
	for (;;) {
		Token t; Token_get(t);
		if (t.code < TOKEN_AND || t.code > TOKEN_MOD_2) {
			token_back();
			break;
		}
		int16 value2 = token_expression5();
		IJB_ERR_CHKB();
		
		/*
		if (value2 == 0 && t.code >= TOKEN_SLASH) { // なぜか __gnu_thumb1_case_sqi (+20byte) が生成される
			command_error(ERR_DIVIDE_BY_ZERO);
			break;
		}
		*/
		switch (t.code) {
			case TOKEN_AND:		value &= value2;	break;
			case TOKEN_XOR:		value ^= value2;	break;
//			case TOKEN_SHIFT_R:	value >>= value2;	break;
//			case TOKEN_SHIFT_L:	value <<= value2;	break;
			case TOKEN_SHIFT_R: // 1.1b6
				if (value2 > 0) {
					//value >>= value2;
					value = (unsigned short)value >> value2; // 1.1b11
				} else {
					value <<= -value2;
				}
				break;
			case TOKEN_SHIFT_L:
				if (value2 > 0) {
					value <<= value2;
				} else {
//					value >>= -value2;
					value = (unsigned short)value >> -value2; // 1.1b11
				}
				break;
			case TOKEN_ASTER:	value *= value2;	break;
			// 整数divを使わないため
			case TOKEN_SLASH:
			case TOKEN_MOD_1:
			case TOKEN_MOD_2:
				if (value2 == 0) {
					command_error(ERR_DIVIDE_BY_ZERO);
					break;
				}
				if (t.code == TOKEN_SLASH) // r0, r1 同時に求まることを利用して削減可能
					value = calcDiv(value, value2);
				else
					value = calcMod(value, value2);
				break;
		}
	}
	return value;
}
static int16 token_paren1() {
	int16 v = token_expression();
	IJB_ERR_CHK1(v);
	Token t; Token_get(t);
	if (t.code != TOKEN_PAREN_E) {
		command_error(ERR_SYNTAX_ERROR);
	}
	return v;
}
static int16 token_opt1() {
	Token t; Token_get(t);
	int16 v = 0;
	if (t.code != TOKEN_PAREN_E) {
		token_back();
		v = token_expression();
		if (!_g.err) {
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
			}
		}
	}
	return v;
}
//#define LIMIT_POS_CALCSTACK (ram + SIZE_RAM + 288 * 2) // 2段分確保 P=PEEK([0])でエラー
#define LIMIT_POS_CALCSTACK (ram + SIZE_RAM + 288 * 1) // 1段分確保

static int16 token_expression5() {
	Token t; Token_get(t);
	int n;
//	printf("%d %d\n", t.code, TOKEN_IOT_IN); // , 32
	switch (t.code) {
		case TOKEN_MINUS:
			return -token_expression5();
		case TOKEN_NOT:
			return ~token_expression5();
		case TOKEN_LNOT_1:
		case TOKEN_LNOT_2:
			return !token_expression5();
		case TOKEN_NUMBER:
			return t.value;
		case TOKEN_VAR:
			return var[t.value];
		case TOKEN_ARRAY:
			return var[token_getArrayIndex()];
		case TOKEN_PAREN_B: {
			int16 v = token_expression();
			IJB_ERR_CHKB();
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			return v;
		}
		case TOKEN_INKEY:
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			n = key_getKey();
			if (!n) // 1.2b19
				return 0x100;
			if (n < 0)
				return 0;
			return n;
		case TOKEN_BTN:
			return IJB_btn(token_opt1());
		case TOKEN_POS: { // 1.3b8 + 72byte
			/* // size一緒
			switch (token_opt1()) {
				case 1: return _g.cursorx;
				case 2: return _g.cursory;
				case 3: return _g.screenw;
				case 4: return _g.screenh;
				default: return _g.cursorx + _g.cursory * _g.screenw;
			}
			*/
			int n = token_opt1();
			if (n == 1) {
				return _g.cursorx;
			}
			if (n == 2) {
				return _g.cursory;
			}
			if (n == 3) {
				return _g.screenw;
			}
			if (n == 4) {
				return _g.screenh;
			}
			return _g.cursorx + _g.cursory * _g.screenw;
		}
		case TOKEN_SOUND:
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			return psg_sound();
		case TOKEN_ANA:
			return IJB_ana(token_opt1());
		case TOKEN_FREE:
//			return (uint16)((uint)__get_MSP() - (uint)&_ebss); // RAM調べ 1.2b13 824byte
			return (IJB_SIZEOF_LIST - 2) - _g.listsize; // 1024byte化
//			return IJB_SIZEOF_LIST - _g.listsize;
		case TOKEN_VER: {
			int n = token_opt1();
			if (n == 0)
				return IJB_VER * 100 + IJB_BUILD;
			if (n == 3)
				return LANG;
			if (n == 4)
				return PSG_TICK_PER_SEC;
			if (n == 2) {
				return key_getKeyboardID();
				/*
				#ifdef KEY_LAYOUT_US
					return 0; // us
				#else
					return 1; // jp
				#endif
				*/
			}
//			if (n == 1) { // +24byte LANGから差分、+14byte
//				return 0; // platform: LPC1114
//			}
			return VER_PLATFORM; // n ==1 も 0なので
		}
//		case TOKEN_LANG: // -8byte
//			return LANG;
		case TOKEN_LEN: {
			int16 n = token_paren1();
			if (n >= OFFSET_RAMROM) {
				char* s = (char*)(ram + n - OFFSET_RAMROM);
				n = 0;
				for (;;) {
					if (*s == '"' || *s == '\0' || s >= (char*)ram + SIZE_RAM)
						break;
					s++;
					n++;
				}
			} else {
				n = 0;
			}
			return n;
		}
		case TOKEN_TICK:
			return video_tick(token_opt1());
		case TOKEN_FILE:
			return IJB_file();
		case TOKEN_LINE: {
			char* pc2 = pc;
			if (pc2 < list || pc2 >= list + SIZE_RAM_LIST) {
				pc2 = pcbreak;
			}
			if (pc2 >= list) {
				int index = 0;
				for (;;) {
					int n = list_getNumber(index);
					int size = list_getLength(index);
					if (pc2 < list + index + size + 4) {
						return n;
					}
					if (n == 0)
						break;
					index += size + 4;
				}
			}
			return 0;
		}
		case TOKEN_LEFT:
		case TOKEN_RIGHT:
		case TOKEN_UP:
		case TOKEN_DOWN:
		case TOKEN_SPACE:
			return t.code - (TOKEN_LEFT - 28);
		case TOKEN_ABS: {
			int16 v = token_paren1();
			return v < 0 ? -v : v;
		}
		case TOKEN_RND:
			return IJB_random(token_paren1());
			//return IJB_random(token_opt1()); // 1.3.2b12 ?? RND() だけにしたいけど
		case TOKEN_PEEK_1:
		case TOKEN_PEEK_2:
			return IJB_peek(token_paren1());
		case TOKEN_SIN:
		case TOKEN_COS: {
			int16 v = token_paren1();
			if (t.code == TOKEN_COS)
				v += 90;
			return sin360(v);
		}
		/*
		case TOKEN_COS: {
			int16 v = token_paren1();
			return (int16)cos32(v);
		}
		case TOKEN_TAN: {
			int16 v = token_paren1();
			return (int16)tan32(v);
		}
		*/
		/*
		case TOKEN_ASC: {
	//		int16 v = token_paren1();
	//		if (v >= OFFSET_RAMROM) {
	//			v = *(char*)(ram + v - OFFSET_RAMROM);
	//		} else {
	//			v = 0;
	//		}
	//		return v;

			Token_get(t);
			if (t.code != TOKEN_STRING) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			int16 v = (uint8)*token_skipstr();
			if (_g.err)
				break;
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			return v;
		}
			*/
		case TOKEN_IN: {
			int v = token_opt1();
			int in = IJB_in();
			if (!v) {
				return in; // 個別に呼んだほうが速い、アナログのせい
			}
			return (in & (1 << (v - 1))) != 0;
		}
		case TOKEN_VPEEK: // +12byte
		case TOKEN_SCR:
		case TOKEN_POINT: { // 1.3.2b19
			int type = t.code;
			Token_get(t);
			if (t.code == TOKEN_PAREN_E) {
				return screen_getCurrent();
			}
			token_back();
			int16 v = token_expression();
			IJB_ERR_CHK1(0);
			Token_get(t);
			if (t.code != TOKEN_COMMA) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			int16 v2 = token_expression();
			IJB_ERR_CHK1(0);
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			if (type == TOKEN_POINT)
				return screen_pset(v, v2, 3); // POINT
			return screen_get(v, v2);
		}
		// 2 params
		case TOKEN_USR: {
			int16 v = token_expression();
			int16 v2 = 0; // 1.3.2b22 省略可能に
			IJB_ERR_CHK1(0);
			Token_get(t);
			if (t.code == TOKEN_COMMA) {
				v2 = token_expression();
				IJB_ERR_CHK1(0);
				Token_get(t);
			}
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			return IJB_usr(v, v2);
		}
		/*
		case TOKEN_ATAN2: {
			int16 v = token_expression();
			if (_g.err)
				return 0;
			Token_get(t);
			if (t.code != TOKEN_COMMA) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			int16 v2 = token_expression();
			if (_g.err)
				return 0;
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			return atan2(v, v2);
		}
		*/
		//
		case TOKEN_I2CR:
		case TOKEN_I2CW: {
			/*
			i2cw(i2cad, adw, adwlen, adw2, adw2len)
			i2cw(i2cad, adw, adwlen)
			i2cr(i2cad, adw, adwlen, adr, adrlen)
			i2cr(i2cad, adr, adrlen)
			*/
//			int writemode = t.code == TOKEN_I2CW;
			int writemode = t.code == TOKEN_I2CR;
			uint16 n[5];
			for (int i = 0;; i++) {
				n[i] = token_expression();
				IJB_ERR_CHK1(0);
				if (i == 4)
					break;
				Token_get(t);
				if (t.code != TOKEN_COMMA) {
					if (i == 2) { // 1.2b42 i2c省略形対応
						token_back();
						if (writemode) {
							n[3] = n[1];
							n[4] = n[2];
							n[2] = 0;
						} else {
							n[3] = n[4] = 0;
						}
						break;
					} else if (i == 3 || (i == 1 && !writemode)) { // 1.2b42
						token_back();
						if (i == 3) {
							n[4] = n[3];
							n[3] = n[2];
						} else { // i == 1
							n[3] = n[4] = 0;
						}
						n[2] = 1;
						ram[OFFSET_RAM_I2CBUF] = n[1];
						n[1] = OFFSET_RAM_I2CBUF + OFFSET_RAMROM;
						break;
					}
					command_error(ERR_SYNTAX_ERROR);
					return 0;
				}
			}
			Token_get(t);
			if (t.code != TOKEN_PAREN_E) {
				command_error(ERR_SYNTAX_ERROR);
				break;
			}
			int res = IJB_i2c(writemode, n);
			if (res == 2) {
				command_error(ERR_ILLEGAL_ARGUMENT);
				break;
			}
			return res;
		}
		case TOKEN_STRING: { // ver 1.2
			return token_skipstr() - (char*)ram + OFFSET_RAMROM;
		}
		case TOKEN_AT: { // ver 1.2b8
			int16 index = 0;
			char* label = pc - 1;
			for (;;) {
				int16 num = list_getNumber(index);
				if (!num)
					break;
				char* s = list + index + 3;
				if (*s == '@') {
					char* p = label;
					for (;;) {
						// @AB3
						char c = *s++;

						// 前方一致 ver 1.2.1
//						if (c == ':' || c == 0 || c == '\'') {
						if (c == ':' || c == 0 || c == '\'' || c == ' ') { // 1.2b43 空白を追加
							pc = p;
							return num;
						}
						if (c != *p++)
							break;

						/*
						// 全文一致 ver 1.2beta 20byte増
						char c2 = *p++; // 12byte up 前方一致->全文一致 @A+1 などが効かなくなるので廃止
						if ((c == ':' || c == 0 || c == ' ') && (c2 == ':' || c2 == 0 || c2 == ' ')) {
							pc = p - 1;
							return num;
						}
						if (c != c2)
							break;
						*/
					}
				}
				index += list_getLength(index) + 4;
			}
			command_error(ERR_UNDEFINED_LINE);
			break;
		}
		#ifdef EXT_IOT
		case TOKEN_IOT_IN: {
			return iot_in();
		}
		#endif
		default: {
			command_error(ERR_SYNTAX_ERROR);
			break;
		}
	}
	return 0;
}

// command ------------------------------------------------------------------------

void command_rem() {
	while (*pc)
		pc++;
}
void command_let(char token) {
	Token t; Token_get(t);
	int16 v = 0;
	switch (t.code) {
		case TOKEN_VAR:
			v = t.value;
			break;
		case TOKEN_ARRAY:
			v = token_getArrayIndex();
			IJB_ERR_CHK();
			if (token == TOKEN_COMMA) {
				Token_get(t);
				if (t.code != token) {
					command_error(ERR_SYNTAX_ERROR);
					return; // IJB_ERR_RETURN();
				}
				command_let2(v);
				for (;;) {
					IJB_ERR_CHK(); // 1.2b31
					Token_get(t);
					if (t.code != token) {
						token_back();
						token_end();
						return;
					}
//					if (++v >= IJB_SIZEOF_ARRAY + IJB_SIZEOF_VAR) {
					if (++v >= IJB_SIZEOF_ARRAY) { // 1.1b10
						command_error(ERR_INDEX_OUT_OF_RANGE);
						IJB_ERR_RETURN();
					}
					command_let2(v);
				}
			}
			break;
		default:
			command_error(ERR_SYNTAX_ERROR);
			return; // IJB_ERR_RETURN();
	}
	Token_get(t);
	if (t.code != token) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	command_let2(v);
	IJB_ERR_CHK(); // 1.2b31
	token_end();
}
int command_let2(int v) {
	int16 value = token_expression();
	IJB_ERR_CHK1(0);
#ifdef BOOT_CHECK
	if (bootflg)
		value = 0;
#endif
	return var[v] = value;
}
void command_if() {
	int b = token_expression();
	IJB_ERR_CHK();
#ifdef BOOT_CHECK
	if (bootflg)
		b = !b;
#endif
	if (b) {
		Token t; Token_get(t);
		if (t.code != TOKEN_THEN)
			token_back();
	} else {
		for (;;) {
			int code = token_getCode();
			if (code == TOKEN_NULL) {
				if (*pc == '\0')
					break;
				pc++;
			} else if (code == TOKEN_STRING) {
				token_skipstr();
			//} else if (code == TOKEN_ELSE || code == TOKEN_ERROR) { // 1.2b9
			} else if (code == TOKEN_ELSE) { //  || code == TOKEN_ERROR) { // 1.4b10 ERRORを無視 絵文字付きラベルへの対応
				break;
			} else if (code == TOKEN_IF || code == TOKEN_REM_1 || code == TOKEN_REM_2) { // 1.2b9
				command_rem();
				break;
			}
		}
	}
}
void command_for() {
	if (_g.nforstack >= IJB_SIZEOF_FOR_STACK) {
//		command_error(ERR_TOO_MANY_NESTED_FORS);
		command_error(ERR_STACK_OVERFLOW);
		return; // IJB_ERR_RETURN();
	}
#ifdef MEM_UNDER64KB
	forstack[_g.nforstack++] = (pchar)(uint)pc;
#else
	forstack[_g.nforstack++] = pc;
#endif

	Token t; Token_get(t);
	int16 v = 0;
	if (t.code == TOKEN_VAR) {
		v = t.value;
	} else if (t.code == TOKEN_ARRAY) {
		v = token_getArrayIndex();
		IJB_ERR_CHK();
	} else {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	Token_get(t);
	if (t.code != TOKEN_EQ && t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int ival = command_let2(v);
	IJB_ERR_CHK();

	Token_get(t);
	if (t.code != TOKEN_TO) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int to = token_expression(); // to
	IJB_ERR_CHK();

	int step = 1;
	Token_get(t);
	if (t.code != TOKEN_STEP) {
		token_back();
	} else {
		step = token_expression(); // step
		IJB_ERR_CHK();
	}
	if ((step > 0 && ival > to) || (step < 0 && ival < to)) {
		command_error(ERR_ILLEGAL_ARGUMENT);
		return; // IJB_ERR_RETURN();
	}
	token_end();
}
void command_next() {
	if (_g.nforstack == 0) {
//		command_error(ERR_NEXT_WITHOUT_FOR);
		command_error(ERR_NOT_MATCH);
		return; // IJB_ERR_RETURN();
	}
	token_end();
	char* bkpc = pc;
#ifdef MEM_UNDER64KB
	pc = (char*)((uint)forstack[_g.nforstack - 1] | POS_RAM);
#else
	pc = forstack[_g.nforstack - 1];
#endif

	Token t; Token_get(t);
	int v = 0;
	if (t.code == TOKEN_VAR) {
		v = t.value;
	} else if (t.code == TOKEN_ARRAY) {
		v = token_getArrayIndex();
		IJB_ERR_CHK();
	} else {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	Token_get(t);
	if (t.code != TOKEN_EQ && t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int ival = token_expression(); // initval
	IJB_ERR_CHK();

	Token_get(t);
	if (t.code != TOKEN_TO) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int to = token_expression(); // to
	IJB_ERR_CHK();

	int step = 1;
	Token_get(t);
	if (t.code != TOKEN_STEP) {
		token_back();
	} else {
		step = token_expression(); // step
		IJB_ERR_CHK();
	}
	token_end();

	if (var[v] == to) {
		pc = bkpc;
		_g.nforstack--;
		return;
	} else {
		if (ival <= to) {
			if ((int16)(var[v] + step) > to) {
				pc = bkpc;
				_g.nforstack--;
				return;
			}
		} else if ((int16)(var[v] + step) < to) {
			pc = bkpc;
			_g.nforstack--;
			return;
		}
	}
	var[v] += step;
}
S_INLINE void command_goto() {
	int n = token_expression(); // 式だとrenumで都合が悪い
	IJB_ERR_CHK();
	n = list_findGoto(n);
	if (n < 0) {
		command_error(ERR_UNDEFINED_LINE);
		return; // IJB_ERR_RETURN();
	}
	token_end();
	list_setPC(n);
}
S_INLINE void command_gosub() {
	if (_g.ngosubstack >= IJB_SIZEOF_GOSUB_STACK) {
//		command_error(ERR_TOO_MANY_NESTED_GOSUBS);
		command_error(ERR_STACK_OVERFLOW);
		return; // IJB_ERR_RETURN();
	}
	int n = token_expression();
	IJB_ERR_CHK();
	n = list_findGoto(n);
	if (n < 0) {
		command_error(ERR_UNDEFINED_LINE);
		return; // IJB_ERR_RETURN();
	}
	token_end();

#ifdef MEM_UNDER64KB
	gosubstack[_g.ngosubstack++] = (pchar)(uint)pc;
#else
	gosubstack[_g.ngosubstack++] = pc;
#endif

	list_setPC(n);
}
S_INLINE void command_return() {
	if (_g.ngosubstack == 0) {
//		command_error(ERR_RETURN_WITHOUT_GOSUB);
		command_error(ERR_NOT_MATCH);
		return; // IJB_ERR_RETURN(); // 逆に増える
	}
	token_end();
#ifdef MEM_UNDER64KB
	pc = (char*)((uint)gosubstack[--_g.ngosubstack] | POS_RAM);
#else
	pc = gosubstack[--_g.ngosubstack];
#endif
}
S_INLINE void command_cont() {
	token_end();
	if (pc < list || pc >= list + 1026) {
		pc = pcbreak;
	}
	if (pc >= list) {
		int index = 0;
		for (;;) {
			int n = list_getNumber(index);
			int size = list_getLength(index);
//			_printf("line %n %x %x %x\n", n, pc, list, list + index + size + 4);

			if (pc < list + index + size + 4) {
				n = list_findGoto(n);
				if (n < 0) {
					command_error(ERR_UNDEFINED_LINE);
					return; // IJB_ERR_RETURN();
				}
				list_setPC(n);
				break;
			}
			if (n == 0)
				break;
			index += size + 4;
		}
	}
}
void command_print() {
	int retflg = 1;
	for (;;) {
		Token t; Token_get(t);
		if (t.code == TOKEN_NULL || t.code == TOKEN_ELSE) {
			token_back();
			break;
		}
		switch (t.code) {
			case TOKEN_STRING:
				token_puts();
				break;
			case TOKEN_STR: {
				int16 n = token_expression();
				IJB_ERR_CHK();
				int16 m = -1;
				Token_get(t);
				if (t.code == TOKEN_COMMA) {
					m = token_expression();
					IJB_ERR_CHK();
					Token_get(t);
				}
				if (t.code != TOKEN_PAREN_E) {
					command_error(ERR_SYNTAX_ERROR);
					return;
				}
				put_strmem(n, m);
				break;
			}
			case TOKEN_CHR:
				for (;;) {
					int16 n = token_expression();
					IJB_ERR_CHK();
					put_chr((char)(n & 0xff));
					Token_get(t);
					if (t.code == TOKEN_COMMA) {
						continue;
					}
					if (t.code != TOKEN_PAREN_E) {
						command_error(ERR_SYNTAX_ERROR);
						return;
					}
					break;
				}
				break;
			case TOKEN_DEC: {
				int n2 = token_expression();
				IJB_ERR_CHK();
				int16 m = 0;
				Token_get(t);
				if (t.code == TOKEN_COMMA) {
					m = token_expression();
					IJB_ERR_CHK();
					Token_get(t);
				}
				if (t.code != TOKEN_PAREN_E) {
					command_error(ERR_SYNTAX_ERROR);
					return;
				}
				if (m <= 0) {
					put_num(n2);
				} else {
					// -10000, 3 -> 000
					// 3333, 5 [ 3333]
					int beam = getBeam(n2);
					if (beam <= m) {
						for (int i = m - beam; i > 0; i--)
							put_chr(' ');
						put_num(n2);
					} else {
						if (n2 < 0)
							n2 = -n2;
						beam = 5;
						for (uint d = 10000; d > 0; d /= 10) {
							//uint c = (uint)n2 / d;
							uint c = calcDivU(n2, d);
							if (beam <= m) {
								put_chr(c + '0');
							}
							n2 -= c * d;
							beam--;
						}
					}
				}
				break;
			}
			case TOKEN_HEX: {
				uint16 n2 = token_expression() & 0xffff;
				IJB_ERR_CHK();
				int16 m = 0;
				Token_get(t);
				if (t.code == TOKEN_COMMA) {
					m = token_expression();
					IJB_ERR_CHK();
					Token_get(t);
				}
				if (t.code != TOKEN_PAREN_E) {
					command_error(ERR_SYNTAX_ERROR);
					return;
				}
				if (m == 0) {
					uint16 n3 = n2;
					for (;;) {
						m++;
						n3 >>= 4;
						if (!n3)
							break;
					}
				}
				for (int i = m - 1; i >= 0; i--) {
					uint h = (n2 >> (i * 4)) & 0xf;
					if (h >= 10) {
						put_chr(h + ('A' - 10));
					} else {
						put_chr(h + '0');
					}
				}
				break;
			}
			case TOKEN_BIN: {
				uint16 n2 = token_expression() & 0xffff;
				IJB_ERR_CHK();
				int16 m = 0;
				Token_get(t);
				if (t.code == TOKEN_COMMA) {
					m = token_expression();
					IJB_ERR_CHK();
					Token_get(t);
				}
				if (t.code != TOKEN_PAREN_E) {
					command_error(ERR_SYNTAX_ERROR);
					return;
				}
				if (m == 0) {
					uint16 n3 = n2;
					for (;;) {
						m++;
						n3 >>= 1;
						if (!n3)
							break;
					}
				}
				for (int i = m - 1; i >= 0; i--) {
					put_chr('0' + ((n2 >> i) & 0x1));
				}
				break;
			}
			case TOKEN_ERROR:
				command_error(ERR_SYNTAX_ERROR);
				break;
			default:
				token_back();
				int16 n = token_expression();
				IJB_ERR_CHK();
				put_num(n);
				break;
		}
		retflg = 1;
		Token_get(t);
		if (t.code == TOKEN_NULL || t.code == TOKEN_ELSE) {
			token_back();
			break;
		}
		/*
		switch (t.code) {
			case TOKEN_COMMA:
//				put_chr('\t');
				put_chr(' '); // 1.4b07
				break;
			case TOKEN_SEMICOLON:
				retflg = 0;
				break;
			default:
				command_error(ERR_SYNTAX_ERROR);
				return;
		}
		*/
		if (t.code == TOKEN_COMMA) {
//		put_chr('\t');
			put_chr(' '); // 1.4b07
		} else if (t.code == TOKEN_SEMICOLON) {
			retflg = 0;
		} else {
			command_error(ERR_SYNTAX_ERROR);
			return;
		}
	}
	if (retflg)
		put_chr('\n');
	token_end();
}

// command

static int token_option1(int m) {
	IJB_ERR_CHK1();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		token_back();
	} else {
		m = token_expression();
		IJB_ERR_CHK1();
	}
	token_end();
	return m;
}

/*
loopしない版 = IJB_DONT_LOOP

1, command_input で、一旦処理を終えて、返す 4?
2. キー入力ごとの処理をする IJB_input
	esc で、break
	enterで、入力ラインを解析して、続きを実行

*/
#ifdef IJB_DONT_LOOP
char* bkp_input;
int16 v_input;
#endif

S_INLINE void command_input() { // 配列の場合2回評価される問題あり? -> 1.2b48で解消? INPUT[0]でエラーバグ修正
	Token t; Token_get(t);
	if (t.code == TOKEN_STRING) {
		token_puts();
		Token_get(t);
		if (t.code != TOKEN_COMMA) {
			command_error(ERR_SYNTAX_ERROR);
			return;
		}
		Token_get(t);
	} else {
		put_chr('?');
	}
	int16 v;
	if (t.code == TOKEN_VAR) {
		v = t.value;
	} else if (t.code == TOKEN_ARRAY) {
		v = token_getArrayIndex();
		IJB_ERR_CHK();
	} else {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	token_end();
	IJB_ERR_CHK();

	if (_g.cursory < 0) { // 1.4b14
		command_error(ERR_NOT_MATCH); // not match
		return;
	}


#ifdef IJB_DONT_LOOP
	v_input = v;
	bkp_input = pc;
#else
	char* line = NULL;
	IJB_input(&line);
	if (stopExecute()) {
		return;
	}
	errorignore = 1; // エラー無視
	char* bkp = pc;
	pc = line;

	command_let2(v);
	token_end();
	errorignore = 0;
	pc = bkp;

	put_chr('\n');
#endif
}
#ifdef IJB_DONT_LOOP
static void command_input2(char* line) {
	errorignore = 1; // エラー無視
	pc = line;

	command_let2(v_input);
	token_end();
	errorignore = 0;
	pc = bkp_input;

	put_chr('\n');
}
#endif

S_INLINE void command_new() {
	token_end();
	IJB_ERR_CHK();
	memclear4((uint8*)list, IJB_SIZEOF_LIST);
	_g.listsize = 0;
	pc = pcbreak = NULL;
}

#ifdef IJB_DONT_LOOP
int continue_list;
int bk_index_list;
int bk_cnt_list;
uint bk_max_list;
int bk_min_list;
#endif

S_INLINE void command_list() {
	#ifdef IJB_DONT_LOOP
	continue_list = 0;
	#endif

	int min = 0;
	uint max = 0;

	if (token_getChar()) {
		min = token_expression();
		IJB_ERR_CHK();
		switch (token_getCode()) {
			case TOKEN_COMMA:
				max = token_expression();
				IJB_ERR_CHK();  // ,の後省略でその後全部とか?、0の場合、その後全部 b20
				break;
			case TOKEN_NULL:
			case TOKEN_ELSE:
				if (min < 0) { // -指定でその行まで ・・・入らない b20
					max = -min;
					min = 0;
				} else {
					max = min;
				}
//				min = 0; // 第二省略時にその行まで表示 // 1.2b19 その行だけに試しに変更
				token_back();
				break;
			default:
				command_error(ERR_SYNTAX_ERROR);
				return;
		}
	}
	if (!max) {
		// どれも空容量は変わらない
//		--max; // = -1; //0x7fff;
		max = 1 << 14; // 16384までは標準表示 16384 = 16 * 1024
//		max = 30000; // 30000までは標準表示

		if (min > max) { // 開始が16385以上の時はそれ以降を表示 1.2b44
			max = -1;
		}
	}

	token_end();
	IJB_ERR_CHK();

	// same as screen.h
//#define SCREEN_W 32
//#define SCREEN_H 24

	int index = 0;
	int cnt = 0;
	for (;;) {
		int num = list_getNumber(index);
//		xprintf("num %d %d\n", num, max);
		if (num == 0 || num > max)
			break;
		if (num >= min) {
			uint len = getBeam(list_getNumber(index)) + strlen8(list + index + 3) + 2;
			//len = len / SCREEN_W + 1;
			len = calcDivU(len, SCREEN_W) + 1;
//			len = (len >> 5) + 1; // 容量、変化なし、自動で最適化していると思われる
			cnt += len;
			if (cnt >= SCREEN_H - 1) { // 1.2.1 -2 -> -1
				IJB_wait(40, 1);
				cnt = len;
//				if (stopExecute()) {
//					return;
//				}
				#ifdef IJB_DONT_LOOP
				continue_list = 1;
				bk_index_list = index;
				bk_cnt_list = cnt;
				bk_max_list = max;
				bk_min_list = bk_min_list;
				return;
				#endif
			}
			if (stopExecute()) { // 行単位で停まるように変更 1.4b07
				return;
			}

			put_num(list_getNumber(index));
			put_chr(' ');
			put_str(list + index + 3);
			put_chr('\n');
		}
		index += list_getLength(index) + 4;
	}
//	xprintf("done\n");
}

/*
loopしない版 = IJB_DONT_LOOP

1, command_list で、停止するか、最後まで終えて返す BASIC_RESULT_LIST
2. listmodeのときは、command_list_next を読み出し、返り値を新しい listmode の値とする
*/

#ifdef IJB_DONT_LOOP
int command_list_next() {
	if (!continue_list) {
		return 0;
	}
	if (stopExecute()) {
		return 0;
	}
	int index = bk_index_list;
	int cnt = bk_cnt_list;
	uint max = bk_max_list;
	int min = bk_min_list;

	put_num(list_getNumber(index));
	put_chr(' ');
	put_str(list + index + 3);
	put_chr('\n');
	
	index += list_getLength(index) + 4;
	for (;;) {
		int num = list_getNumber(index);
//		xprintf("num %d %d\n", num, max);
		if (num == 0 || num > max)
			break;
		if (num >= min) {
			uint len = getBeam(list_getNumber(index)) + strlen8(list + index + 3) + 2;
			//len = len / SCREEN_W + 1;
			len = calcDivU(len, SCREEN_W) + 1;
//			len = (len >> 5) + 1; // 容量、変化なし、自動で最適化していると思われる
			cnt += len;
			if (cnt >= SCREEN_H - 1) { // 1.2.1 -2 -> -1
				IJB_wait(40, 1);
				bk_index_list = index;
				bk_cnt_list = len;
				bk_max_list = max;
				bk_min_list = bk_min_list;
				return 1;
//				if (stopExecute()) {
//					return;
//				}
			}
			if (stopExecute()) { // 行単位で停まるように変更 1.4b07
				return 0;
			}

			put_num(list_getNumber(index));
			put_chr(' ');
			put_str(list + index + 3);
			put_chr('\n');
		}
		index += list_getLength(index) + 4;
	}
//	xprintf("done\n");
	return 0;
}
#endif


//renum 番号置き換え対応版 500byte増える -> 勘違い、intを割り算してたからだった
S_INLINE int16 list_findIndex(int16 number) { // for command_renum
	int index = 0;
	int cnt = 0;
	for (;;) {
		int n = list_getNumber(index);
		if (n == 0 || n >= number)
			break;
		index += list_getLength(index) + 4;
		cnt++;
	}
	return cnt;
}
S_INLINE void command_renum2(int index, int n, int step) {
	char* bkpc = pc;

	pc = (char*)(list + index + 3);
	/*
	int len2 = list_getLength(index);
	if (!*(pc + len2 - 1))
		len2--;
	*/
	uint len = strlen8(pc);
//	int len = list_getLength(index); // alignなし版
	for (;;) {
		Token t; Token_get(t);
		if (t.code == TOKEN_NULL) {
			if (*pc == '\0')
				break;
			pc++;
		} else if (t.code == TOKEN_STRING) {
			token_skipstr();
		} else if (t.code == TOKEN_REM_1 || t.code == TOKEN_REM_2 || t.code == TOKEN_ERROR) {
			break;
		} else if (t.code == TOKEN_GOTO || t.code == TOKEN_GOSUB_1 || t.code == TOKEN_GOSUB_2) {
			char* tokenstart = lasttoken;
			Token t; Token_get(t);
			if (t.code == TOKEN_NUMBER) {
				int oldnum = t.value;
				//a Token_get(t);
				//a if (t.code == TOKEN_NULL || t.code == TOKEN_ELSE) {
					uint newnum = n + list_findIndex(oldnum) * step;
					int ob = pc - lasttoken; //getBeam(oldnum);
					int nb = getBeam(newnum);
					//if (ob != nb) {
					//	command_error(ERR_ILLEGAL_ARGUMENT);
					//	return;
					//}
					//a char* pnum = lasttoken;
					//a while (*--pnum == ' ');
					char* pnum = pc - 1;
					// alignの関係でややこしいので動かすのはあきらめ
					if (nb <= ob) {
						nb = ob;
					//a } else if (lasttoken - list - index - 3 < len) { // 何のためだっけ？
					} else { // nb > ob
						if (len & 1) {
							// align分、余裕がある時は1byteだけずらす
							
							//char* p = (char*)(ram + len + 1); //?? おかしかったよね
							char* p = (char*)(list + index + len + 3);
							while (p > pnum) {
								*p = *(p - 1);
								p--;
							}
							pnum++;
							len++;
							ob++;
						}
						/*
						// サイズに余裕があれば伸ばす処理、書きかけ、偶数か奇数か、len & 1 が0か1かで処理異なる
						while (nb > ob && _g.listsize < SIZE_OF_LIST - 2) {
							_g.listsize += 2;
							len += 2;
							ob += 2;
							list_setLength(index, len);
							char* p = (char*)(list + _g.listsize);
							while (p > pnum) {
								*p = *(p - 2);
								p -= 2;
							}
						}
						*/
					}
					for (int i = 0; i < nb && pnum > tokenstart; i++) {
					//a for (int i = 0; i < nb; i++) {
//						pnum[-i] = newnum == 0 && i > 0 ? ' ' : '0' + newnum % 10;
						*pnum = newnum == 0 && i > 0 ? ' ' : '0' + newnum % 10;
						pnum--;
						newnum /= 10;
					}
				//a }
			}
		}
	}
	/*
	// 表示
	put_num(list_getNumber(index));
	put_chr(' ');
	put_str(list + index + 3);
	put_chr('\n');
	*/

	pc = bkpc;
}
void command_renum() {
	int start = 10;
	if (token_getChar()) {
		start = token_expression();
	}
	int step = token_option1(10);
	if (start <= 0 || step <= 0) {
		command_error(ERR_ILLEGAL_ARGUMENT); // 1.3b2
		return;
	}

	int index = 0;
	for (;;) {
		int num = list_getNumber(index);
		if (num == 0)
			break;
		// GOTO / GOSUB の行番号付け替え必要!! 0.9.5 式を使ったので、完全な付け替えは不可能
		command_renum2(index, start, step);
		index += list_getLength(index) + 4;
	}
	index = 0;
	for (;;) {
		int num = list_getNumber(index);
		if (num == 0) {
//			_g.listsize = index;
			break;
		}
		list_setNumber(index, start);
		start += step;
		index += list_getLength(index) + 4;
	}
	/*
	printf("LIST listsize: %d\n", _g.listsize);
	char* p = (char*)(ram + OFFSET_RAM_LIST);
	for (int i = 0; i < 100; i++) {
		char c = *(p + i);
		if (c < 0x20)
			printf("%d ", c);
		else
			printf("[%c] ", c);
	}
	printf("\n");
	*/
}

S_INLINE void command_run() {
	//printf("%d %x\n", SIZE_RAM, SIZE_RAM);
	token_end();

//	basic_clearVar(); // 0.9.4から初期化廃止
//	IJB_out(outbuf = 0);
//	IJB_led(0);

	_g.ngosubstack = 0;
	_g.nforstack = 0;
//	IJB_clt();
	key_clearKey(); // 1.4b09 ?
	if (_g.listsize) {
		list_setPC(0);
	} else {
		pc = pcbreak = NULL;
	}
}
S_INLINE void command_end() {
	token_end();
	pc = pcbreak = NULL;
}
void command_load(int command) {
	int n = IJB_file();
//	if (token_getChar()) {
	int code = token_getCode(); // ELSE対策 入らないので対策しない 1.2b61 -> 1.2b62 入った
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		n = token_expression();
		IJB_ERR_CHK();
	}
	// LRUN n, m
	int m = 0;
	if (command == TOKEN_LRUN) {
		code = token_getCode();
		if (code == TOKEN_COMMA) {
			m = token_expression();
			IJB_ERR_CHK();
		} else {
			token_back();
		}
	}
	token_end();
	IJB_ERR_CHK();

	memclear4((uint8*)list, IJB_SIZEOF_LIST);
	_g.listsize = 0;
	pc = pcbreak = NULL;

	int16 s = IJB_load(n, (uint8*)list, IJB_SIZEOF_LIST - 2, 1);
//	int16 s = IJB_load(n, list, IJB_SIZEOF_LIST);
	if (s >= 0) {
		// 読み込み時にサイズ算出している
		int16 index = 0;
		int16 bknum = 0;
		for (;;) {
			int16 num = list_getNumber(index);
			if (num == 0) {
				s = index;
				break;
			}
			if (num <= bknum)
				return; // _g.err
			bknum = num;
			index += list_getLength(index) + 4;
		}
		if (s >= 0 && command == TOKEN_LOAD && !noresmode) {
			//			_printf("Loaded %dbyte\n", s);
			put_str("Loaded ");
			put_num(s);
			put_str("byte\n");
		}
		_g.listsize = s;

		// run
		if (command == TOKEN_LRUN) {
			_g.ngosubstack = 0;
			_g.nforstack = 0;
//			basic_clearVar();
			if (_g.listsize) {
				if (m > 0) {
					m = list_findGoto(m);
				}
				if (m < 0) {
					command_error(ERR_UNDEFINED_LINE);
					return;
				}
				list_setPC(m);
			}
		}
	} else {
		command_error(ERR_FILE_ERROR);
	}
}
S_INLINE void command_save() {
	int16 n = IJB_file();
//	if (token_getChar()) {
	int code = token_getCode(); // ELSE対策 1.2b61
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		n = token_expression();
		IJB_ERR_CHK();
	}
	token_end();
	IJB_ERR_CHK();

	if (!IJB_save(n, (uint8*)list, _g.listsize)) {
		if (!noresmode) {
			//		_printf("Saved %dbyte\n", _g.listsize);
			put_str("Saved ");
			put_num(_g.listsize);
			put_str("byte\n");
		}
	} else {
		command_error(ERR_FILE_ERROR);
	}
}
/*
FILES -> 0,3
FILES 0 -> 0,227
FILES n -> 0,n
FILES 0,0 -> 0,0
FILES n,m -> n,m
*/
#ifdef FLASH_ONLY_ONE
#define N_FLASH_STORAGE 1
#else
#ifdef ROM_MODE
#define N_FLASH_STORAGE LEN_FLASH
#else
#ifndef N_FLASH_STORAGE
#define N_FLASH_STORAGE 4
#endif
#endif
#endif

#ifdef IJB_DONT_LOOP
uint8 continue_files = 0;
int16 bk_startn_files = 0;
int16 bk_endn_files = 0;
#endif

S_INLINE void command_files() {
	#ifdef IJB_DONT_LOOP
	continue_files = 0;
	#endif

	int16 endn = N_FLASH_STORAGE - 1; // default Flash only
	int16 startn = 0;
	if (token_getChar()) {
		endn = token_expression();
		IJB_ERR_CHK();
		Token t; Token_get(t);
		if (t.code != TOKEN_COMMA) {
			token_back();
			if (endn == 0) { // ver 1.2.4
				endn = 100 + 128 - 1;
			}
		} else {
			startn = endn;
			endn = token_expression();
			IJB_ERR_CHK();
		}
	}
	token_end();
	IJB_ERR_CHK();

//	int max = endn;
	if (endn >= 100) {
		if (!i2c0_init()) { // ?いる?
//			max += 128;
//			max += endn - 100;
		}
	}

	int cnt = 0;
//	int wcnt = 0;
	int16 buf[16];
	for (int i = startn; i <= endn; i++) {
		if (i >= N_FLASH_STORAGE && i < 100)
			continue;
		
		int res = IJB_load(i, (uint8*)buf, SCREEN_W/*32*/, 0);
		/*
		if (res < 0) { // 1.4.2b19
			break;
		}
		*/
		int b = put_num(i);
		if (res == SCREEN_W/*32*/) { // && line > 0) {
//		if (res > 5) { // ver 1.2 beta 35
			int16 line = *buf;
			if (line > 0) {
				put_chr(' ');
				char* p = (char*)buf;
				uint8 len = p[2];
				if (len > SCREEN_W/*32*/ - 3 - b)
					len = SCREEN_W/*32*/ - 3 - b;
				p[len + 3] = '\0';
				put_str(p + 3);
			}
		}
		put_chr('\n');
		cnt++;
//			wcnt = 0;
		if (cnt >= SCREEN_H - 2) {
			IJB_wait(60, 1);
			if (stopExecute()) { // なぜかUARTからのESCでフリーズ
//				IJB_wait(60); // why??
				break;
			}
			#ifdef IJB_DONT_LOOP
			continue_files = 1;
			bk_startn_files = i;
			bk_endn_files = endn;
			return;
			#endif
			cnt = 0;
		}

		/* else {
			wcnt++;
			if (wcnt == 999) { // いくつなら乱れないか？
				if (IJB_wait(1)) { // waitしないと表示が乱れる
					return;
				}
				wcnt = 0;
			}
		}*/
	}
}
#ifdef IJB_DONT_LOOP
int command_files_next() {
	if (!continue_files) {
		return 0;
	}
	if (stopExecute()) {
		continue_files = 0;
		return 0;
	}
	int16 startn = bk_startn_files + 1;
	int16 endn = bk_endn_files;

	int cnt = 0;
//	int wcnt = 0;
	int16 buf[16];
	for (int i = startn; i <= endn; i++) {
		if (i >= N_FLASH_STORAGE && i < 100)
			continue;
		int b = put_num(i);

		int res = IJB_load(i, (uint8*)buf, SCREEN_W/*32*/, 0);
		if (res == SCREEN_W/*32*/) { // && line > 0) {
//		if (res > 5) { // ver 1.2 beta 35
			int16 line = *buf;
			if (line > 0) {
				put_chr(' ');
				char* p = (char*)buf;
				uint8 len = p[2];
				if (len > SCREEN_W/*32*/ - 3 - b)
					len = SCREEN_W/*32*/ - 3 - b;
				p[len + 3] = '\0';
				put_str(p + 3);
			}
		}
		put_chr('\n');
		cnt++;
//			wcnt = 0;
		if (cnt >= SCREEN_H - 2) {
			IJB_wait(60, 1);
			if (stopExecute()) { // なぜかUARTからのESCでフリーズ
//				IJB_wait(60); // why??
				continue_files = 0;
				return 0;
			}
			continue_files = 1;
			bk_startn_files = i;
			return 1;
		}
	}
	continue_files = 0;
	return 0;
}
#endif

//
S_INLINE void command_led() {
	int16 n = token_expression();
	IJB_ERR_CHK();
	IJB_led(n);
	token_end();
}
S_INLINE void command_out() {
	int16 n = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		token_back();
		IJB_out(0, n);
	} else {
		int16 m = token_expression();
		IJB_ERR_CHK();
		IJB_out(n, m);
	}
	token_end();
}
S_INLINE void command_pwm() {
	int16 n = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int16 m = token_expression();
	int16 l = token_option1(0);
	IJB_pwm(n, m, l);
}
S_INLINE void command_clo() {
	IJB_clo();
	token_end();
}
S_INLINE void command_wait() {
	int16 n = token_expression();
	int m = token_option1(1);
	IJB_wait(n, m);
}
/*
S_INLINE void command_lwait() {
	int16 n = token_expression();
	if (_g.err)
		return;
	token_end();
	IJB_waitLittle(n);
}
*/
S_INLINE void command_cls() {
	token_end();
	screen_clear();
}
S_INLINE void command_clt() {
	token_end();
	video_waitSync(1);
	video_clt();
}
S_INLINE void command_clv() {
	token_end();
	basic_clearVars();
}
S_INLINE void command_locate() {
	int16 x = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	int16 y;
	if (code == TOKEN_COMMA) {
		y = token_expression();

		// cursor switch 1.3b4
		code = token_getCode();
		if (code == TOKEN_COMMA) {
			//_g.cursorflg = token_expression() != 0;
			_g.cursorflg = token_expression();
		} else {
			_g.cursorflg = 0;
			token_back();
		}
	} else {
		token_back(); // 20byte 1.3b8
		y = calcDiv(x, _g.screenw); // 8byte増える
		x = calcMod(x, _g.screenw);
//		y = x / _g.screenw;
//		x = x % _g.screenw;
	}

	IJB_ERR_CHK();
	token_end();
	screen_locate(x, y);
}
/*
void command_motor() {
	int16 x = token_expression();
	if (_g.err)
		return;
	Token t; Token_get(t);
	if (t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int16 y = token_expression();
	if (_g.err)
		return;
	IJB_motor(x, y);
	token_end();
}
*/
S_INLINE void command_sleep() {
	IJB_sleep();
	token_end();
}
S_INLINE void command_reset() {
	IJB_reset();
}
S_INLINE void command_bps() {
	int n = token_expression();
	// 1.2b56 I2C BPS
	int m = token_option1(0);
	IJB_bps(n, m);
}
S_INLINE void command_play() {
	/*
	Token t; Token_get(t);
	if (t.code != TOKEN_STRING) {
		psg_playMML(NULL);
		token_back();
	} else {
		char* sp = token_skipstr();
		if (_g.err)
			return;
		psg_playMML(sp);
	}
	token_end();
	*/
	char* mml = NULL;

//	if (token_getChar()) { // ELSE未対策
	int code = token_getCode(); // ELSE対策 1.2b61
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		int16 n = token_expression();
		if (n >= OFFSET_RAMROM) {
			mml = (char*)(ram + n - OFFSET_RAMROM);
		}
	}
	psg_playMML(mml);
	token_end();
}

S_INLINE void command_beep() {
	int16 len = 3;
	int16 tone = 10;
//	if (token_getChar()) {
	int code = token_getCode(); // ELSE対策 1.2b61
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		tone = token_expression();
		IJB_ERR_CHK();
		code = token_getCode();
		if (code != TOKEN_COMMA) {
			token_back();
		} else {
			len = token_expression();
			IJB_ERR_CHK();
		}
	}
	token_end();
	psg_beep(tone, len); // default, psg_beep(10, 3);
}
S_INLINE void command_tempo() {
	int16 tempo = token_expression();
	IJB_ERR_CHK();
	token_end();
	psg_tempo(tempo);
}
S_INLINE void command_video() {
	int16 video = token_expression();
	// 1.1b3
	int clkdiv = token_option1(1);
	//
	if (video) {
		/*
		VIDEO 0 - off
		VIDEO 1 - normal
		VIDEO 2 - invert
		VIDEO 3 - big
		VIDEO 4 - big invert
		*/
		if (video < 0)
			video = 0;
		_g.screen_invert = (video & 1) == 0; // VIDEO2, 4 -> invert
		int big = (video - 1) >> 1;  // VIDEO3, 4 -> big
		if (big > 3) {
			big = 3;
		}
		if (big != _g.screen_big) {
			_g.screen_big = big;
			video_on();
			screen_clear();
		} else {
			video_on();
		}
	} else {
		video_off(clkdiv);
	}
}
S_INLINE void command_scroll() {
	int16 dir = token_expression();
	IJB_ERR_CHK();
	token_end();
	screen_scroll(dir);
}
S_INLINE void command_poke() {
	int16 n1 = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int16 n2 = token_expression();
	IJB_ERR_CHK();
	IJB_poke(n1, n2);
	for (;;) {
		code = token_getCode();
		if (code != TOKEN_COMMA) {
			token_back();
			token_end();
			return;
		}
		n1++;
		n2 = token_expression();
		IJB_ERR_CHK();
		IJB_poke(n1, n2);
	}
}
/*
S_INLINE void command_pset() {
	int16 x = token_expression();
	if (_g.err)
		return;
	Token t; Token_get(t);
	if (t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int16 y = token_expression();
	if (_g.err)
		return;
	Token_get(t);
	if (t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int16 n = token_expression();
	if (_g.err)
		return;
	screen_locate(x >> 1, y >> 1);
	int p = screen_getCurrent();
	if (n) {
		p |= (1 << ((x & 1) | ((y & 1) << 1)));
	} else {
		p &= ~(1 << ((x & 1) | ((y & 1) << 1)));
	}
	screen_putc(p);
	token_end();
}
*/
S_INLINE void command_copy() {
	int16 dst = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int16 src = token_expression();
	IJB_ERR_CHK();
	code = token_getCode();
	if (code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	}
	int16 len = token_expression();
	IJB_ERR_CHK();

//	for (int i = 0; i < len; i++)
//		IJB_poke(dst + i, IJB_peek(src + i));
	if (len > 0) { // len マイナスで 逆方向、入らない
		for (int i = 0; i < len; i++)
			IJB_poke(dst++, IJB_peek(src++));
	} else {
		for (int i = 0; i > len; i--)
			IJB_poke(dst--, IJB_peek(src--));
//		for (int i = -len - 1; i >= 0; i--)
//			IJB_poke(dst + i, IJB_peek(src + i));
	}
	token_end();
}
S_INLINE void command_clp() {
	screen_clp();
	token_end();
}
S_INLINE void command_clk() {
	key_clearKey();
	token_end();
}
// 21120 -> 21236 (+114) limit 24576

	/*
#000-#6FF	キャラクターパターン #00〜#DF (読み取り専用)
#700-#7FF	書換可能キャラクターパターン(PCG) #E0〜#FF
#800-#8FF	2byte符号付き整数の変数（配列0〜101、変数A〜Z）
#900-#BFF	画面キャラクターコード(VRAM) 32x24
#C00-#FFF	プログラム（行番号(2byte)、サイズ(1byte)、テキスト）
#1000-		キーバッファ（・・・これとは限らないことにしよう、プログラム領域拡大予約のため） // b16
*/
S_INLINE void command_help() {
//	put_str("MEM MAP\n#000 CHAR\n#700 PCG\n#800 VAR\n#900 VRAM\n#C00 LIST\n"); // b14 一時的に削除 ver 1.1正式版
	put_str("#000 CHAR\n#700 PCG\n#800 VAR\n#900 VRAM\n#C00 LIST\n"); // 1.2b58, 1.3b4
//	put_str("MEM MAP\n#000 CHAR\n#700 PCG\n#800 VAR\n#900 VRAM\n#C00 LIST\n"); // 1.2b62

//	put_str("MEM MAP\nCHAR #000\nPCG  #700\nVAR  #800\nVRAM #900\nLIST #C00\n"); // "LIST\n#1002 KEY\n"
//	put_str("MEM MAP\nCHAR #000-#6FF\nPCG  #700-#7FF\nVAR  #800-#8FF\nVRAM #900-#BFF\nLIST #C00-#FFF\n"); // "LIST\n#1002 KEY\n"
//	put_str("MEM MAP\n#0 CHAR\n#700 PCG\n#800 VAR\n#900 VRAM\n#C00 LIST\n#1002 KEY\n"); // b14 一時的に削除
//	put_str("MEMMAP\n#000-#6FF CHAR(ROM)\n#700-#7FF CHAR(RAM)\n#800-#8FF VAR\n#900-#BFF VRAM\n#C00-#FFF LIST\n#1002 KEY\n");
	//	put_str("MEM MAP\n#700 PCG\n#800 VAR\n#900 VRAM\n#C00 LIST\n"); // "\n#1002-#100 KEYBUF\n"
	token_end();
}

S_INLINE void command_srnd() {
	int16 n = token_expression();
	IJB_ERR_CHK();
	token_end();
	IJB_random_seed(n);
}

S_INLINE void command_uart() {
	int16 n = token_expression();
	int16 m = token_option1(1);
	IJB_uart(n, m);
}
S_INLINE void command_at() {
//	while (*pc && *pc != ':' && *pc != '\'') {
	while (*pc && *pc != ':') { // 1.2b40
		pc++;
	}
	token_end();
}

S_INLINE void command_lcd() { // 1.2b32, 1.3.2b12 contrast 292->248 +44byte
	uint n = !_g.display_mode;
	int code = token_getCode(); // ELSE対策 1.2b61
	token_back();
	if (code != TOKEN_NULL && code != TOKEN_ELSE) {
		n = token_expression();
		IJB_ERR_CHK();

		int code = token_getCode();
		if (code != TOKEN_COMMA) {
			token_back();
		} else {
			n += token_expression();
			IJB_ERR_CHK();
		}
	}
	token_end();
	IJB_lcd(n);
}

S_INLINE void command_ok() { // 1.2b42 表示オフ追加、b53で OK0からOK2に変更
	int n = 0;
	if (token_getChar()) {
//		n = !token_expression(); // 1.2b42 OK0で表示OFF
		n = token_expression() == 2;	// 1.2b53 OK2で表示OFF
		IJB_ERR_CHK();
	}
	token_end();
	noresmode = n;
	
	/*
	if (pc < list || pc >= list + IJB_SIZEOF_LIST) { // プログラム実行時でなければ何もしない
		_g.err = -1;
	}
	*/
	if (pc >= list + IJB_SIZEOF_LIST) { // プログラム実行時でなければ何もしない
		_g.err = -1; // 表示しないエラーメッセージ
	}
}

// 1

// sin値+1 （先頭除く）
static const unsigned char SIN_TABLE[] = { 0, 3, 8, 12, 17, 21, 26, 30, 35, 39, 43, 48, 52, 57, 61, 65, 70, 74, 78, 82, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127, 131, 135, 138, 142, 146, 149, 153, 157, 160, 164, 167, 170, 174, 177, 180, 183, 186, 189, 192, 195, 198, 201, 203, 206, 209, 211, 214, 216, 218, 221, 223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 241, 242, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255, 255 };

int sin360(int deg) {
	int pm = 1;
	if (deg < 0) {
		deg = -deg;
		pm = -pm;
	}
	while (deg > 360)
		deg -= 360;
	if (deg > 180) {
		deg -= 180;
		pm = -pm;
	}
	if (deg > 90)
		deg = 180 - deg;
	if (!deg)
		return 0;
	return pm * (SIN_TABLE[deg] + 1);
}

/*
// 切り捨て、誤差多い
const uint8_t SIN_TABLE[] = { 0, 4, 8, 13, 17, 22, 26, 31, 35, 40, 44, 48, 53, 57, 61, 66, 70, 74, 79, 83, 87, 91, 95, 100, 104, 108, 112, 116, 120, 124, 127, 131, 135, 139, 143, 146, 150, 154, 157, 161, 164, 167, 171, 174, 177, 181, 184, 187, 190, 193, 196, 198, 201, 204, 207, 209, 212, 214, 217, 219, 221, 223, 226, 228, 230, 232, 233, 235, 237, 238, 240, 242, 243, 244, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255 }; // 90byte

int sin(int deg) { // 680, 20byte多いけど、高速 (int16にすると12byte増える)
	int pm = 1;
	if (deg < 0) {
		deg = -deg;
		pm = -pm;
	}
//	deg = calcMod(deg, 360); // 8byte増える、内部はループなので速度は遅い
	while (deg > 360)
		deg -= 360;
	if (deg > 180) {
		pm = -pm;
		deg -= 180;
	}
	if (deg == 90)
		return pm * 256;
	if (deg > 90)
		deg = 180 - deg;
//	if (deg > 85) // 8byte増える、table追加したほうが小さく、速い
//		deg = 85;
	return pm * SIN_TABLE[deg];
}
*/

/*
S_INLINE int sin(int deg) { // 700
	int x = 0;
	int y = 0;
	int v = 37000000;
	int pm = 1;
	if (deg < 0) {
		deg = -deg;
		pm = -pm;
	}
//	deg = calcMod(deg, 360);
	while (deg > 360)
		deg -= 360;
	//deg %= 360;
	if (deg > 180) {
		pm = -pm;
		deg -= 180;
	}
	if (deg > 90) {
		deg = 180 - deg;
	}
	while (x != deg) {
		x++;
		y += v;
		//v -= y / 3283;
		v -= calcDiv(y, 3283);
	}
//	return y / 212008 * pm; // +-10,000

//	return y / 2120080 * pm; // +-1,000
//	return calcDiv(y, 2120080) * pm;

//	return y / 8281562 * pm; // +-256
	return pm * calcDiv(y, 8281562);
}
*/

/*
// -- sin/cos/tan from @yrm

int32 sin32(int32 s) {
	int32 x = 0;
	int32 y = 0;
	int32 v = 37000000;

	int8 pm = 1;
	if (s < 0) {
		s = -s;
		pm = -pm;
	}
	s %= 360;
	if (s > 180) {
		pm = -pm;
		s -= 180;
	}
	if (s > 90) {
		s = 180 - s;
	}

	while (x != s) {
		x++;
		y += v;
		v -= y / 3283;
	}
	return (y / 212008) * pm; // +-10,000
//	return (y / 2120080) * pm; // +-1,000
}

S_INLINE int32 cos32(int32 s) {
	return sin32(s + 90);
}
S_INLINE int32 tan32(int32 s) {
	int32 c = cos32(s);
	return c == 0 ? INT32_MAX : sin32(s) * 10000 / c; // +-10,000
//	return c == 0 ? INT32_MAX : sin32(s) * 1000 / c; // +-1,000
}

// atan2

// http://labs.techfirm.co.jp/iappli/910


// https://ponherm.wordpress.com/2011/03/17/atan2-%E8%BF%91%E4%BC%BC%E9%96%A2%E6%95%B0/
// y,x は 4096 =1 で正規化されていること
int atan2internal(int y, int x) {
	int atan2;
	if (y > x) {
		atan2 = 5900 - x;
	} else {
		atan2 = y;
	}
	return atan2 << 12 / 5900;
}
int atan2(int y, int x) {
	int radius;
	if (y >= 0) {
		if (x < 0) {
			radius = atan2internal(-x, y) + 0x1000;
		} else {
			radius = atan2internal(y, x) + 0x0000;
		}
	} else {
		if (x < 0) {
			radius = atan2internal(-y, -x) + 0x2000;
		} else {
			radius = atan2internal(x, -y) + 0x3000;
		}
	}
	return radius;
}
*/

// line
S_INLINE void command_draw() { // +284byte
	int pos[5];
	int i;
	for (i = 0; i < 5; i++) {
		pos[i] = token_expression();
		IJB_ERR_CHK();
		int code = token_getCode();
		if (code != TOKEN_COMMA) {
			break;
		}
	}
	if (i == 0) {
		command_error(ERR_SYNTAX_ERROR);
		return; // IJB_ERR_RETURN();
	} else if (i & 1) {
		pos[i + 1] = 1;
		i++;
	}
	token_end();
	if (i == 2) {
//		token_back();
		screen_pset(pos[0], pos[1], pos[2]);
	} else {
		screen_line(pos[0], pos[1], pos[2], pos[3], pos[4]);
	}
}

#ifdef EXT_SENSEHAT
S_INLINE void command_sh_mlsp() {
printf("mlsp\n");
	int x = token_expression();
	printf("%d\n", x);
	IJB_ERR_CHK();
	Token t; Token_get(t);
	if (t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int y = token_expression();
	printf("%d\n", y);
	IJB_ERR_CHK();
	Token_get(t);
	if (t.code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int z = token_expression();
	printf("%d\n", z);
	IJB_ERR_CHK();
	token_end();
	IJB_led(x);
printf("mlsp %d,%d,%d\n", x, y, z);
}
#endif

#ifdef EXT_IOT
S_INLINE void command_iot_out() {
	int n = token_expression();
	IJB_ERR_CHK();

	int code = token_getCode();
	int m = -1;
	if (code != TOKEN_COMMA) {
		token_back();
		token_end();
		iot_out(n);
	} else {
		m = token_expression();
		IJB_ERR_CHK();
		int flash = token_option1(1);
		token_end();
		iot_out2(n, m, flash);
	}
}
#endif

S_INLINE void command_ws_out(int port) {
	//int p = token_expression();
	int n = token_expression();
	int m = token_option1(1);
	ws_out(port, n, m);
}

#ifndef NO_KBD_COMMAND
#ifdef VERSION15
S_INLINE void command_kbd() {
	int n = token_expression();
	IJB_kbd(n);
}
S_INLINE void command_dac() {
	int16 n1 = token_expression();
	IJB_ERR_CHK();
	int code = token_getCode();
	if (code != TOKEN_COMMA) {
		command_error(ERR_SYNTAX_ERROR);
		return;
	}
	int16 n2 = token_expression();
	IJB_ERR_CHK();
	IJB_dac(n1, n2);
}
#endif
#endif

// __BASIC_H__
#endif
