#define _GNU_SOURCE

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define SYS_FREQ                270

#define SAMPLE_RATE             44100
#define NUM_CHANNELS            2
#define BUFFER_SIZE             1024

#define COLOR_WHITE          0x9dc2
#define COLOR_LIGHTGRAY      0x8d42
#define COLOR_DARKGRAY       0x3306
#define COLOR_BLACK          0x11c2

#define ILI9225_COLOR_WHITE             0x9dc2
#define ILI9225_COLOR_LIGHTGRAY         0x8d42
#define ILI9225_COLOR_DARKGRAY          0x3306
#define ILI9225_COLOR_BLACK             0x11c2

#define SDL2_COLOR_WHITE                0x9bbc0fff     /* White */
#define SDL2_COLOR_LGRAY                0x8bac0fff     /* Light Gray */
#define SDL2_COLOR_DGRAY                0x306230ff     /* Dark Gray */
#define SDL2_COLOR_BLACK                0x0f380fff     /* Black */

#define SYSTEM_CLOCK        4194304
#define LCD_HEIGHT          220
#define LCD_WIDTH           176
#define SCREEN_WIDTH        160
#define SCREEN_HEIGHT       144

#define KiB                 1024
#define MiB                 1048576

#define MSB(n)                      (((uint16_t)(n) >> 8) & 0x00ff)
#define LSB(n)                      ((uint16_t)(n) & 0x00ff)
#define TO_U16(lsb, msb)            (((uint16_t)(msb) << 8) | (uint16_t)(lsb))
#define BIT(f, n)                   (((f) >> (n)) & 0x0001)
#define SET(f, n)                   ((f) |= (1U << (n)))
#define RES(f, n)                   ((f) &= ~(1U << (n)))
#define IN_RANGE(x, a, b)           ((x) >= (a) && (x) <= (b))
#define IS_FALLING_EDGE(a, b)       ((a) && !(b))

#define JOYPAD_A                0
#define JOYPAD_B                1
#define JOYPAD_START            2
#define JOYPAD_SELECT           3
#define JOYPAD_RIGHT            4
#define JOYPAD_UP               5
#define JOYPAD_DOWN             6
#define JOYPAD_LEFT             7

/* registers definition */
#define JOYP_REG_P1                 0xff00

#define TIMER_REG_DIV                 0xff04
#define TIMER_REG_TIMA                0xff05
#define TIMER_REG_TMA                 0xff06
#define TIMER_REG_TAC                 0xff07

#define INTERRUPT_REG_IE                 0xffff
#define INTERRUPT_REG_IF                 0xff0f

#define PPU_REG_LCDC                0xff40
#define PPU_REG_STAT                0xff41
#define PPU_REG_SCY                 0xff42
#define PPU_REG_SCX                 0xff43
#define PPU_REG_LY                  0xff44
#define PPU_REG_LYC                 0xff45
#define PPU_REG_BGP                 0xff47
#define PPU_REG_OBP0                0xff48
#define PPU_REG_OBP1                0xff49
#define PPU_REG_WY                  0xff4a
#define PPU_REG_WX                  0xff4b

#define DMA_REG_OAM                 0xff46

#define STAT_INTR_MODE0             (1U << 3)
#define STAT_INTR_MODE1             (1U << 4)
#define STAT_INTR_MODE2             (1U << 5)
#define STAT_INTR_LYC               (1U << 6)

#define INTERRUPT_SRC_VBLANK     (1U << 0)
#define INTERRUPT_SRC_LCD        (1U << 1)
#define INTERRUPT_SRC_TIMER      (1U << 2)
#define INTERRUPT_SRC_SERIAL     (1U << 3)
#define INTERRUPT_SRC_JOYPAD     (1U << 4)

enum BUS_REGIONS {
    ROM = (1U << 0),
    vRAM = (1U << 1),
    externalRAM = (1U << 2),
    WRAM = (1U << 3),
    ECHO_RAM = (1U << 4),
    OAM = (1U << 5),
    UNUSED = (1U << 6),
    IO = (1U << 7),
    HRAM = (1U << 8),
};

enum IO_REGIONS {
    INTERRUPT = (1U << 0),
    TIMER = (1U << 1),
    PPU = (1U << 2),
    JOYPAD = (1U << 3),
};

typedef enum {
    FRONT,
    BACK 
} which_buffer_t;

typedef enum {
    NORMAL,
    HALT,
    HALT_BUG,
    STOP
} gb_mode_t;

typedef enum {
    HBLANK,
    VBLANK,
    OAM_SCAN,
    DRAWING,
} ppu_mode_t;

typedef enum {
    OFF,
    WAITING,
    TRANSFERING,
} dma_mode_t;

typedef enum {
    SQUARE1 = 1,
    SQUARE2 = 2,
    WAVE    = 3,
    NOISE   = 4,
    CTRL    = 5,
} apu_channel_t;

typedef enum {
    OBP0,
    OBP1,
    BGP,
} palette_t;

typedef enum {
    BG_WIN,
    SPRITE,
} pixel_type_t;

typedef enum {
    NO_MBC = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    MBC3_TIMER_BATTERY = 0x0f,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
} rom_type_t;

static uint8_t mbc1BitMask[] = {
    [2]   = 0b00000001,
    [4]   = 0b00000011,
    [8]   = 0b00000111,
    [16]  = 0b00001111,
    [32]  = 0b00011111,
    [64]  = 0b00011111,
    [128] = 0b00011111,
};

const uint16_t ili9225Palette[4] = {ILI9225_COLOR_WHITE, ILI9225_COLOR_LIGHTGRAY, ILI9225_COLOR_DARKGRAY, ILI9225_COLOR_BLACK};
const uint32_t sdl2Palette[4] = {SDL2_COLOR_WHITE, SDL2_COLOR_LGRAY, SDL2_COLOR_DGRAY, SDL2_COLOR_BLACK};

struct cpu {
    bool ime;
    union {
        uint16_t val;
        struct {
            union {
                uint8_t f;
                struct {
                    uint8_t unused0 : 1;
                    uint8_t unused1 : 1;
                    uint8_t unused2 : 1;
                    uint8_t unused3 : 1;
                    uint8_t c : 1;
                    uint8_t h : 1;
                    uint8_t n : 1;
                    uint8_t z : 1;
                } flag;
            };
            uint8_t a;
        };
    } af;

    union {
        uint16_t val;
        struct {
            uint8_t c;
            uint8_t b;
        };
    } bc;

    union {
        uint16_t val;
        struct {
            uint8_t e;
            uint8_t d;
        };
    } de;

    union {
        uint16_t val;
        struct {
            uint8_t l;
            uint8_t h;
        };
    } hl;

    uint16_t sp;
    uint16_t pc;
};

struct cartridge {
    struct rom {
        uint8_t *data;
        char name[17];
        uint8_t type;
        int size;
        int bankNumber;
    } rom;
    struct ram {
        uint8_t data[32 * KiB];
        int size;
    } ram;
};

typedef enum ACCESS_MODE {
    READ,
    WRITE,
    DUMMY,
} access_mode_t;

struct memory_access_record {
    uint16_t addr;
    uint16_t val;
    access_mode_t mode;
};

struct interrupt {
    uint8_t ie;
    uint8_t flag;
    bool interruptHandled;
};

struct timer {
    uint16_t div : 14;
    uint8_t tima;
    uint8_t tma;
    union {
        uint8_t val;
        struct {
            uint8_t freq : 2;
            uint8_t enable : 1;
            uint8_t unused : 5;
        };
    } tac;
};

struct oam_entry {
    uint8_t y;
    uint8_t x;
    uint8_t tileIndex;
    union {
        uint8_t val;
        struct {
            uint8_t cgbPalette : 3;
            uint8_t bank : 1;
            uint8_t dmgPalette : 1;
            uint8_t xFlip : 1;
            uint8_t yFlip : 1;
            uint8_t priority : 1;
        };
    } attributes;
};

struct ppu {
    bool frameReady;
    struct {
        uint8_t val;
        bool bgWinEnable;
        bool objEnable;
        bool winEnable;
        bool ppuEnable;
        uint8_t objSize : 5;
        uint16_t bgTileMap;
        uint16_t bgWinTiles;
        uint16_t winTileMap;
    } lcdc;
    union {
        uint8_t val;
        struct {
            uint8_t ppuMode : 2;
            uint8_t lycEqualLy : 1;
            uint8_t mode0IntrSelect : 1;
            uint8_t mode1IntrSelect : 1;
            uint8_t mode2IntrSelect : 1;
            uint8_t lycIntrSelect : 1;
            uint8_t unused : 1;
        };
    } stat;
    uint8_t scy;
    uint8_t scx;
    uint8_t ly;
    uint8_t lyc;
    uint8_t pal[3];
    uint8_t wy;
    int8_t wx;
    uint16_t ticks;
    ppu_mode_t mode;
    bool scanLineReady;
    struct oam_entry oamEntry[10];
    uint8_t oamEntryCounter : 4;
    uint8_t spriteCounter : 4;
    bool statIntrLine;
    bool windowInFrame;
    int windowLineCounter;
    bool drawWindowThisLine;
};

struct dma {
    int tick;
    dma_mode_t mode;
    uint16_t reg;
    uint16_t startAddr;
};

struct joypad {
    union {
        uint8_t val;
        struct {
            uint8_t keys : 4;
            uint8_t selectDPad : 1;
            uint8_t selectButton : 1;
            uint8_t unused : 2;
        };
    } joyp;
    bool button[8];
};

struct mbc1{
    bool ramEnable;
    uint8_t romBank : 5;
    uint8_t ramBank : 2;
    bool bankingMode;
    bool hasBattery;
};

struct mbc3 {
    bool ramEnable;
    uint8_t romBank : 7;
    uint8_t ramBank : 2;
    bool hasBattery;
};

struct mbc {
    struct mbc1 mbc1;
    struct mbc3 mbc3;
};

struct serial {
    uint8_t sb;
    uint8_t sc;
};

struct gb {
    which_buffer_t whichBuffer; 
    uint16_t *frontBufferPtr;
    uint16_t *backBufferPtr;
    uint8_t vRAM[0x2000];
    uint8_t externalRAM[8 * KiB];
    uint8_t workRAM[0x2000];
    uint8_t echoRAM[0x1e00];
    uint8_t oam[0xa0];
    uint8_t unused[0x60];
    uint8_t highRAM[0x7f];
    gb_mode_t mode;
    struct cpu cpu;
    struct cartridge cart;
    struct interrupt interrupt;
    struct timer timer;
    struct ppu ppu;
    struct dma dma;
    struct joypad joypad;
    struct mbc mbc;
    struct serial serial;
    int executedCycle;
};

const uint16_t palette[4] = {COLOR_WHITE, COLOR_LIGHTGRAY, COLOR_DARKGRAY, COLOR_BLACK};

const int timerClockFrequency[] = {
    [0] = 1024,
    [1] = 16,
    [2] = 64,
    [3] = 256
};

const uint8_t instrCycle[256] = {
//  x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1, // 0x
    1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1, // 1x
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 2x
    2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 3x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 4x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 5x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 6x
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 7x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 8x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 9x
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // Ax
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // Bx
    2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4, // Cx
    2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 1, 3, 6, 2, 4, // Dx
    3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4, // Ex
    3, 3, 2, 1, 0, 4, 2, 4, 4, 2, 4, 1, 0, 0, 2, 4, // Fx
};

const uint8_t cbInstrCycle[256] = {
//  x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 1x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 2x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 3x 
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 4x 
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 5x 
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 6x 
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 7x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 8x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 9x 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Ax 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Bx 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Cx 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Dx 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Ex 
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // Fx 
};

#define INTERRUPT_REQUEST(intr_src)                     \
    gb->interrupt.flag |= intr_src

#define IS_INTERRUPT_PENDING()                          \
    (gb->interrupt.ie & gb->interrupt.flag & 0x1f)

#define GET_MEM_REGION(addr)                    \
    ((IN_RANGE(addr, 0x0000, 0x7fff) << 0)|     \
    (IN_RANGE(addr, 0x8000, 0x9fff) << 1) |     \
    (IN_RANGE(addr, 0xa000, 0xbfff) << 2) |     \
    (IN_RANGE(addr, 0xc000, 0xdfff) << 3) |     \
    (IN_RANGE(addr, 0xe000, 0xfdff) << 4) |     \
    (IN_RANGE(addr, 0xfe00, 0xfe9f) << 5) |     \
    (IN_RANGE(addr, 0xfea0, 0xfeff) << 6) |     \
    (IN_RANGE(addr, 0xff00, 0xff7f) << 7) |     \
    (IN_RANGE(addr, 0xff80, 0xfffe) << 8) |     \
    ((addr == 0xffff) << 7))

#define IS_INTERRUPT_REG(addr)                          \
    (addr == INTERRUPT_REG_IE || addr == INTERRUPT_REG_IF)

#define IS_TIMER_REG(addr)                              \
    (addr == TIMER_REG_DIV || addr == TIMER_REG_TAC ||      \
    addr == TIMER_REG_TIMA || addr == TIMER_REG_TMA)

#define IS_PPU_REG(addr)                                                        \
    (addr == PPU_REG_LCDC || addr == PPU_REG_STAT || addr == PPU_REG_SCY ||     \
    addr == PPU_REG_SCX || addr == PPU_REG_LY || addr == PPU_REG_LYC ||         \
    addr == PPU_REG_BGP || addr == PPU_REG_OBP0 || addr == PPU_REG_OBP1 ||      \
    addr == PPU_REG_WY || addr == PPU_REG_WX)

#define WHICH_IO_REGION(addr)                   \
    ((IS_INTERRUPT_REG(addr) << 0)        |     \
    (IN_RANGE(addr, 0xff04, 0xff07) << 1) |     \
    (IN_RANGE(addr, 0xff40, 0xff4b) << 2) |     \
    ((addr == JOYP_REG_P1) << 3))

#define SET_MODE(Mode)              \
    gb->ppu.stat.ppuMode = Mode;   \
    gb->ppu.mode = Mode

/* MBC declarations */
uint8_t mbc1_read(struct gb *gb, uint16_t addr);
void mbc1_write(struct gb *gb, uint16_t addr, uint8_t val);

/* bus declarations */
uint8_t dma_get_data(struct gb *gb, uint16_t addr);
uint8_t bus_read(struct gb *gb, uint16_t addr);
void bus_write(struct gb *gb, uint16_t addr, uint8_t val);

/* interrupt declarations */
uint8_t interrupt_read(struct gb *gb, uint16_t addr);
void interrupt_write(struct gb *gb, uint16_t addr, uint8_t val);
void interrupt_process(struct gb *gb);
void interrupt_request(struct gb *gb, uint8_t intr_src);
bool is_interrupt_pending(struct gb *gb);

/* timer declarations */
uint8_t timer_read(struct gb *gb, uint16_t addr);
void timer_write(struct gb *gb, uint16_t addr, uint8_t val);

/* CPU declarations */
void cpu_step(struct gb *gb);
void cpu_init(struct gb *gb);
void cpu_cycle(struct gb *gb, int cycles);

/* PPU declarations */
int cmpfunc(const void *a, const void *b);
uint8_t ppu_read(struct gb *gb, uint16_t addr);
void ppu_write(struct gb *gb, uint16_t addr, uint8_t val);
void ppu_draw_scanline(struct gb *gb);

/**********************************************************************************************/
/************************************* CPU related parts **************************************/
/**********************************************************************************************/

#define CPU_FETCH_BYTE()                   \
    bus_read(gb, gb->cpu.pc++)

#define CPU_FETCH_WORD(ret)            \
    lsb = CPU_FETCH_BYTE();            \
    msb = CPU_FETCH_BYTE();            \
    ret = TO_U16(lsb, msb)

uint16_t sm83_fetch_word(struct gb *gb)
{
    uint8_t lsb = CPU_FETCH_BYTE();
    uint8_t msb = CPU_FETCH_BYTE();

    return TO_U16(lsb, msb);
}

#define CPU_PUSH_BYTE(val)             \
    bus_write(gb, --gb->cpu.sp, val)

void cpu_push_word(struct gb *gb, uint16_t val)
{
    CPU_PUSH_BYTE(MSB(val));
    CPU_PUSH_BYTE(LSB(val));
}

uint8_t cpu_pop_byte(struct gb *gb)
{
    gb->cpu.sp++;
    return bus_read(gb, gb->cpu.sp - 1);
}

uint16_t cpu_pop_word(struct gb *gb)
{
    uint8_t lsb = cpu_pop_byte(gb);
    uint8_t msb = cpu_pop_byte(gb);

    return TO_U16(lsb, msb);
}

#define TOGGLE_ZNH(res, fn, fh)             \
    gb->cpu.af.flag.z = !(uint8_t)(res);      \
    gb->cpu.af.flag.n = (bool)(fn);           \
    gb->cpu.af.flag.h = (bool)(fh)

void toggle_znh(struct gb *gb, uint8_t res, bool n, bool h)
{
    gb->cpu.af.flag.z = !res;
    gb->cpu.af.flag.n = n;
    gb->cpu.af.flag.h = h;
}

/* CPU Instructions */

#define LD_INDIRECT_HL_N(n)     \
    bus_write(gb, gb->cpu.hl.val, n)

#define LDH_INDIRECT_C_A()      \
    bus_write(gb, 0xff00 + gb->cpu.bc.c, gb->cpu.af.a)

#define LDH_A_INDIRECT_C()      \
    gb->cpu.af.a = bus_read(gb, 0xff00 + gb->cpu.bc.c)

#define LDH_INDIRECT_N_A(n)     \
    bus_write(gb, 0xff00 + n, gb->cpu.af.a)

#define LDH_A_INDIRECT_N(n)     \
    gb->cpu.af.a = bus_read(gb, 0xff00 + n)

#define LD_INDIRECT_NN_SP(nn)   \
    bus_write(gb, nn, LSB(gb->cpu.sp));   \
    bus_write(gb, nn + 1, MSB(gb->cpu.sp))

#define PUSH_RR(rr)     \
    cpu_push_word(gb, rr)

#define LD_HL_SP_PLUS_I8(i8)                                            \
    gb->cpu.hl.val = gb->cpu.sp + (int8_t)i8;                           \
    gb->cpu.af.flag.z = 0;                                              \
    gb->cpu.af.flag.n = 0;                                              \
    gb->cpu.af.flag.h = BIT((gb->cpu.sp + i8) ^ gb->cpu.sp ^ i8, 4);    \
    gb->cpu.af.flag.c = BIT((gb->cpu.sp + i8) ^ gb->cpu.sp ^ i8, 8)

#define ADD(b, d)                                   \
    res = gb->cpu.af.a + b + d;                     \
    carryPerBit = res ^ gb->cpu.af.a ^ b;         \
    gb->cpu.af.a = res;                             \
    TOGGLE_ZNH(res, 0, BIT(carryPerBit, 4));  \
    gb->cpu.af.flag.c = BIT(carryPerBit, 8)       

#define SUB(b, d)                                                                                               \
    res = gb->cpu.af.a + ~b + 1 - d;                \
    carryPerBit = res ^ gb->cpu.af.a ^ ~b;        \
    gb->cpu.af.a = res;                             \
    TOGGLE_ZNH(res, 1, !BIT(carryPerBit, 4)); \
    gb->cpu.af.flag.c = !BIT(carryPerBit, 8);    

#define CP(b)                                                                                                   \
    TOGGLE_ZNH(gb->cpu.af.a + ~b + 1, 1, !BIT(gb->cpu.af.a ^ ~b ^ (gb->cpu.af.a + ~b + 1), 4));             \
    gb->cpu.af.flag.c = !BIT(gb->cpu.af.a ^ ~b ^ (gb->cpu.af.a + ~b + 1), 8)

#define INC_R(r)                                            \
    TOGGLE_ZNH(r + 1, 0, BIT((r + 1) ^ r ^ 1, 4));      \
    r += 1

#define INC_INDIRECT_HL()                                               \
    operand = bus_read(gb, gb->cpu.hl.val);                             \
    bus_write(gb, gb->cpu.hl.val, operand + 1);                         \
    TOGGLE_ZNH(operand + 1, 0, BIT((operand + 1) ^ operand ^ 1U, 4))

#define DEC_R(r)                                               \
    TOGGLE_ZNH(r - 1, 1, !BIT((r - 1) ^ r ^ 0xff, 4));     \
    r--;

#define DEC_INDIRECT_HL()                                               \
    operand = bus_read(gb, gb->cpu.hl.val);                             \
    bus_write(gb, gb->cpu.hl.val, operand - 1);                         \
    TOGGLE_ZNH(operand - 1, 1, !BIT((operand - 1) ^ operand ^ 0xff, 4))

#define AND(b)                          \
    gb->cpu.af.a &= b;                  \
    TOGGLE_ZNH(gb->cpu.af.a, 0, 1); \
    gb->cpu.af.flag.c = 0

#define OR(b)                              \
    gb->cpu.af.a |= b;                     \
    TOGGLE_ZNH(gb->cpu.af.a, 0, 0);    \
    gb->cpu.af.flag.c = 0 

#define XOR(b)                             \
    gb->cpu.af.a ^= b;                     \
    TOGGLE_ZNH(gb->cpu.af.a, 0, 0);    \
    gb->cpu.af.flag.c = 0 

#define CCF()                               \
    gb->cpu.af.flag.n = 0;                  \
    gb->cpu.af.flag.h = 0;                  \
    gb->cpu.af.flag.c = !gb->cpu.af.flag.c

#define SCF()               \
    gb->cpu.af.flag.n = 0;  \
    gb->cpu.af.flag.h = 0;  \
    gb->cpu.af.flag.c = 1

#define DAA()                                                   \
    a = gb->cpu.af.a;                                           \
    if (!gb->cpu.af.flag.n) {                                   \
        if (gb->cpu.af.flag.h || (gb->cpu.af.a & 0x0f) > 0x09)  \
            a += 0x06;                                          \
        if (gb->cpu.af.flag.c || gb->cpu.af.a > 0x99) {         \
            a += 0x60;                                          \
            gb->cpu.af.flag.c = 1;                              \
        }                                                       \
    } else {                                                    \
        if (gb->cpu.af.flag.h)                                  \
            a -= 0x06;                                          \
        if (gb->cpu.af.flag.c)                                  \
            a -= 0x60;                                          \
    }                                                           \
    gb->cpu.af.flag.z = !a;                                     \
    gb->cpu.af.flag.h = 0;                                      \
    gb->cpu.af.a = a;                                           \

#define CPL()                       \
    gb->cpu.af.a = ~gb->cpu.af.a;   \
    gb->cpu.af.flag.n = 1;          \
    gb->cpu.af.flag.h = 1

#define INC_RR(rr)              \
    rr++

#define DEC_RR(rr)              \
    rr--

#define ADD_HL_RR(rr)                                                           \
    gb->cpu.af.flag.n = 0;                                                      \
    gb->cpu.af.flag.h = BIT((gb->cpu.hl.val + rr) ^ gb->cpu.hl.val ^ rr, 12);   \
    gb->cpu.af.flag.c = BIT((gb->cpu.hl.val + rr) ^ gb->cpu.hl.val ^ rr, 16);   \
    gb->cpu.hl.val = gb->cpu.hl.val + rr                                    


#define ADD_SP_I8(i8)                                                   \
    gb->cpu.af.flag.z = gb->cpu.af.flag.n = 0;                          \
    gb->cpu.af.flag.h = BIT((gb->cpu.sp + i8) ^ gb->cpu.sp ^ i8, 4);    \
    gb->cpu.af.flag.c = BIT((gb->cpu.sp + i8) ^ gb->cpu.sp ^ i8, 8);    \
    gb->cpu.sp = gb->cpu.sp + (int8_t)i8

#define RLCA()      \
    gb->cpu.af.flag.c = BIT(gb->cpu.af.a, 7);   \
    gb->cpu.af.a = (gb->cpu.af.a << 1) | gb->cpu.af.flag.c; \
    gb->cpu.af.flag.z = gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define RRCA()          \
    gb->cpu.af.flag.c = BIT(gb->cpu.af.a, 0);   \
    gb->cpu.af.a = (gb->cpu.af.a >> 1) | (gb->cpu.af.flag.c << 7);  \
    gb->cpu.af.flag.z = gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0 

#define RLA()           \
    operand = BIT(gb->cpu.af.a, 7); \
    gb->cpu.af.a = (gb->cpu.af.a << 1) | gb->cpu.af.flag.c; \
    gb->cpu.af.flag.c = operand;    \
    gb->cpu.af.flag.z = gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define RRA()               \
    operand = BIT(gb->cpu.af.a, 0);                                 \
    gb->cpu.af.a = (gb->cpu.af.a >> 1) | (gb->cpu.af.flag.c << 7);  \
    gb->cpu.af.flag.c = operand;           \
    gb->cpu.af.flag.z = gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0 

#define RLC_R(r)                        \
    gb->cpu.af.flag.c = BIT(r, 7);      \
    r = (r << 1) | gb->cpu.af.flag.c;   \
    gb->cpu.af.flag.z = !(r);           \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define RLC_INDIRECT_HL()                           \
    operand = bus_read(gb, gb->cpu.hl.val);         \
    gb->cpu.af.flag.c = BIT(operand, 7);            \
    operand = (operand << 1) | gb->cpu.af.flag.c;   \
    bus_write(gb, gb->cpu.hl.val, operand);         \
    gb->cpu.af.flag.z = !operand;                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define RRC_R(r)                                \
    gb->cpu.af.flag.c = BIT(r, 0);              \
    r = (r >> 1) | (gb->cpu.af.flag.c << 7);    \
    gb->cpu.af.flag.z = !(r);                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define RRC_INDIRECT_HL()                       \
    operand = bus_read(gb, gb->cpu.hl.val);     \
    gb->cpu.af.flag.c = BIT(operand, 0);        \
    operand = (operand >> 1) | (gb->cpu.af.flag.c << 7);    \
    bus_write(gb, gb->cpu.hl.val, operand);     \
    gb->cpu.af.flag.z = !operand;               \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0  

#define RL_R(r)                             \
    operand = BIT(r, 7);                    \
    r = (r << 1) | gb->cpu.af.flag.c;       \
    gb->cpu.af.flag.z = !(r);               \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0;  \
    gb->cpu.af.flag.c = operand

#define RL_INDIRECT_HL()                        \
    val = bus_read(gb, gb->cpu.hl.val);         \
    operand = BIT(val, 7);                      \
    val = (val << 1) | gb->cpu.af.flag.c;       \
    bus_write(gb, gb->cpu.hl.val, val);         \
    gb->cpu.af.flag.z = !val;                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0;  \
    gb->cpu.af.flag.c = operand

#define RR_R(r)                                 \
    operand = BIT(r, 0);                        \
    r = (r >> 1) | (gb->cpu.af.flag.c << 7);    \
    gb->cpu.af.flag.z = !r;                     \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0;  \
    gb->cpu.af.flag.c = operand

#define RR_INDIRECT_HL()                    \
    val = bus_read(gb, gb->cpu.hl.val);     \
    operand = BIT(val, 0);                  \
    val = (val >> 1) | (gb->cpu.af.flag.c << 7);    \
    bus_write(gb, gb->cpu.hl.val, val);     \
    gb->cpu.af.flag.z = !val;               \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0;      \
    gb->cpu.af.flag.c = operand

#define SLA_R(r)                                \
    gb->cpu.af.flag.c = BIT(r, 7);              \
    r <<= 1;                                    \
    gb->cpu.af.flag.z = !r;                     \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define SLA_INDIRECT_HL()                       \
    val = bus_read(gb, gb->cpu.hl.val);         \
    gb->cpu.af.flag.c = BIT(val, 7);            \
    val <<= 1;                                  \
    bus_write(gb, gb->cpu.hl.val, val);         \
    gb->cpu.af.flag.z = !val;                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define SRA_R(r)                                \
    gb->cpu.af.flag.c = BIT(r, 0);              \
    r = (r & 0x80) | (r >> 1);                  \
    gb->cpu.af.flag.z = !r;                     \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define SRA_INDIRECT_HL()                       \
    val = bus_read(gb, gb->cpu.hl.val);         \
    gb->cpu.af.flag.c = BIT(val, 0);            \
    val = (val & 0x80) | (val >> 1);            \
    bus_write(gb, gb->cpu.hl.val, val);         \
    gb->cpu.af.flag.z = !val;                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define SWAP_R(r)                               \
    r = (r >> 4) | (r << 4);                    \
    TOGGLE_ZNH(r, 0, 0);                    \
    gb->cpu.af.flag.c = 0

#define SWAP_INDIRECT_HL()                      \
    val = bus_read(gb, gb->cpu.hl.val);         \
    val = (val >> 4) | (val << 4);              \
    bus_write(gb, gb->cpu.hl.val, val);         \
    TOGGLE_ZNH(val, 0, 0);                  \
    gb->cpu.af.flag.c = 0

#define SRL_R(r)                    \
    gb->cpu.af.flag.c = BIT(r, 0);  \
    r >>= 1;                        \
    gb->cpu.af.flag.z = !r;         \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define SRL_INDIRECT_HL()                       \
    val = bus_read(gb, gb->cpu.hl.val);         \
    gb->cpu.af.flag.c = BIT(val, 0);            \
    val >>= 1;                                  \
    bus_write(gb, gb->cpu.hl.val, val);         \
    gb->cpu.af.flag.z = !val;                   \
    gb->cpu.af.flag.n = gb->cpu.af.flag.h = 0

#define BIT_N_R(n, r)                   \
    TOGGLE_ZNH(BIT(r, n), 0, 1)

#define BIT_N_INDIRECT_HL(n)                    \
    val = bus_read(gb, gb->cpu.hl.val);         \
    TOGGLE_ZNH(BIT(val, n), 0, 1)

#define RES_N_R(n, r)                           \
    RES(r, n)

#define RES_N_INDIRECT_HL(n)                    \
    val = bus_read(gb, gb->cpu.hl.val);         \
    RES(val, n);                                \
    bus_write(gb, gb->cpu.hl.val, val)

#define SET_N_R(n, r)                           \
    SET(r, n)

#define SET_N_INDIRECT_HL(n)                    \
    val = bus_read(gb, gb->cpu.hl.val);         \
    SET(val, n);                                \
    bus_write(gb, gb->cpu.hl.val, val)

#define JP(nn, offset, cond)                    \
    if (cond) {                                 \
        gb->executedCycle += 1;                \
        gb->cpu.pc = nn + (int8_t)offset;       \
    }                                           

#define CALL(nn, cond)                          \
    if (cond) {                                 \
        cpu_push_word(gb, gb->cpu.pc);         \
        gb->cpu.pc = nn;                        \
        gb->executedCycle += 3;                \
    }

#define RET(opcode, cond)                       \
    if (cond) {                                 \
        uint16_t pc = cpu_pop_word(gb);        \
        gb->cpu.pc = pc;                        \
        gb->executedCycle += 3;                \
    }

#define RETI()                      \
    RET(0xc9, 1);                   \
    gb->cpu.ime = true

#define RST_N(n)                    \
    cpu_push_word(gb, gb->cpu.pc); \
    gb->cpu.pc = n

#define HALT()                                                          \
    gb->mode = HALT

#define DI()                    \
    gb->cpu.ime = false

#define EI()                    \
    gb->cpu.ime = true

int execute_cb_instructions(struct gb *gb, uint8_t opcode)
{
    uint8_t operand, val;

    switch (opcode) {
    case 0x00: RLC_R(gb->cpu.bc.b);                              break;
    case 0x01: RLC_R(gb->cpu.bc.c);                              break;
    case 0x02: RLC_R(gb->cpu.de.d);                              break;
    case 0x03: RLC_R(gb->cpu.de.e);                              break;
    case 0x04: RLC_R(gb->cpu.hl.h);                              break;
    case 0x05: RLC_R(gb->cpu.hl.l);                              break;
    case 0x06: RLC_INDIRECT_HL();                                break;
    case 0x07: RLC_R(gb->cpu.af.a);                              break;
    case 0x08: RRC_R(gb->cpu.bc.b);                              break;
    case 0x09: RRC_R(gb->cpu.bc.c);                              break;
    case 0x0a: RRC_R(gb->cpu.de.d);                              break;
    case 0x0b: RRC_R(gb->cpu.de.e);                              break;
    case 0x0c: RRC_R(gb->cpu.hl.h);                              break;
    case 0x0d: RRC_R(gb->cpu.hl.l);                              break;
    case 0x0e: RRC_INDIRECT_HL();                                break;
    case 0x0f: RRC_R(gb->cpu.af.a);                              break;
    case 0x10: RL_R(gb->cpu.bc.b);                               break;
    case 0x11: RL_R(gb->cpu.bc.c);                               break;
    case 0x12: RL_R(gb->cpu.de.d);                               break;
    case 0x13: RL_R(gb->cpu.de.e);                               break;
    case 0x14: RL_R(gb->cpu.hl.h);                               break;
    case 0x15: RL_R(gb->cpu.hl.l);                               break;
    case 0x16: RL_INDIRECT_HL();                                 break;
    case 0x17: RL_R(gb->cpu.af.a);                               break;
    case 0x18: RR_R(gb->cpu.bc.b);                               break;
    case 0x19: RR_R(gb->cpu.bc.c);                               break;
    case 0x1a: RR_R(gb->cpu.de.d);                               break;
    case 0x1b: RR_R(gb->cpu.de.e);                               break;
    case 0x1c: RR_R(gb->cpu.hl.h);                               break;
    case 0x1d: RR_R(gb->cpu.hl.l);                               break;
    case 0x1e: RR_INDIRECT_HL();                                 break;
    case 0x1f: RR_R(gb->cpu.af.a);                               break;
    case 0x20: SLA_R(gb->cpu.bc.b);                              break;
    case 0x21: SLA_R(gb->cpu.bc.c);                              break;
    case 0x22: SLA_R(gb->cpu.de.d);                              break;
    case 0x23: SLA_R(gb->cpu.de.e);                              break;
    case 0x24: SLA_R(gb->cpu.hl.h);                              break;
    case 0x25: SLA_R(gb->cpu.hl.l);                              break;
    case 0x26: SLA_INDIRECT_HL();                                break;
    case 0x27: SLA_R(gb->cpu.af.a);                              break;
    case 0x28: SRA_R(gb->cpu.bc.b);                              break;
    case 0x29: SRA_R(gb->cpu.bc.c);                              break;
    case 0x2a: SRA_R(gb->cpu.de.d);                              break;
    case 0x2b: SRA_R(gb->cpu.de.e);                              break;
    case 0x2c: SRA_R(gb->cpu.hl.h);                              break;
    case 0x2d: SRA_R(gb->cpu.hl.l);                              break;
    case 0x2e: SRA_INDIRECT_HL();                                break;
    case 0x2f: SRA_R(gb->cpu.af.a);                              break;
    case 0x30: SWAP_R(gb->cpu.bc.b);                             break;
    case 0x31: SWAP_R(gb->cpu.bc.c);                             break;
    case 0x32: SWAP_R(gb->cpu.de.d);                             break;
    case 0x33: SWAP_R(gb->cpu.de.e);                             break;
    case 0x34: SWAP_R(gb->cpu.hl.h);                             break;
    case 0x35: SWAP_R(gb->cpu.hl.l);                             break;
    case 0x36: SWAP_INDIRECT_HL();                               break;
    case 0x37: SWAP_R(gb->cpu.af.a);                             break;
    case 0x38: SRL_R(gb->cpu.bc.b);                              break;
    case 0x39: SRL_R(gb->cpu.bc.c);                              break;
    case 0x3a: SRL_R(gb->cpu.de.d);                              break;
    case 0x3b: SRL_R(gb->cpu.de.e);                              break;
    case 0x3c: SRL_R(gb->cpu.hl.h);                              break;
    case 0x3d: SRL_R(gb->cpu.hl.l);                              break;
    case 0x3e: SRL_INDIRECT_HL();                                break;
    case 0x3f: SRL_R(gb->cpu.af.a);                              break;
    case 0x40: BIT_N_R(0, gb->cpu.bc.b);                         break;
    case 0x41: BIT_N_R(0, gb->cpu.bc.c);                         break;
    case 0x42: BIT_N_R(0, gb->cpu.de.d);                         break;
    case 0x43: BIT_N_R(0, gb->cpu.de.e);                         break;
    case 0x44: BIT_N_R(0, gb->cpu.hl.h);                         break;
    case 0x45: BIT_N_R(0, gb->cpu.hl.l);                         break;
    case 0x46: BIT_N_INDIRECT_HL(0);                             break;
    case 0x47: BIT_N_R(0, gb->cpu.af.a);                         break;
    case 0x48: BIT_N_R(1, gb->cpu.bc.b);                         break;
    case 0x49: BIT_N_R(1, gb->cpu.bc.c);                         break;
    case 0x4a: BIT_N_R(1, gb->cpu.de.d);                         break;
    case 0x4b: BIT_N_R(1, gb->cpu.de.e);                         break;
    case 0x4c: BIT_N_R(1, gb->cpu.hl.h);                         break;
    case 0x4d: BIT_N_R(1, gb->cpu.hl.l);                         break;
    case 0x4e: BIT_N_INDIRECT_HL(1);                             break;
    case 0x4f: BIT_N_R(1, gb->cpu.af.a);                         break;
    case 0x50: BIT_N_R(2, gb->cpu.bc.b);                         break;
    case 0x51: BIT_N_R(2, gb->cpu.bc.c);                         break;
    case 0x52: BIT_N_R(2, gb->cpu.de.d);                         break;
    case 0x53: BIT_N_R(2, gb->cpu.de.e);                         break;
    case 0x54: BIT_N_R(2, gb->cpu.hl.h);                         break;
    case 0x55: BIT_N_R(2, gb->cpu.hl.l);                         break;
    case 0x56: BIT_N_INDIRECT_HL(2);                             break;
    case 0x57: BIT_N_R(2, gb->cpu.af.a);                         break;
    case 0x58: BIT_N_R(3, gb->cpu.bc.b);                         break;
    case 0x59: BIT_N_R(3, gb->cpu.bc.c);                         break;
    case 0x5a: BIT_N_R(3, gb->cpu.de.d);                         break;
    case 0x5b: BIT_N_R(3, gb->cpu.de.e);                         break;
    case 0x5c: BIT_N_R(3, gb->cpu.hl.h);                         break;
    case 0x5d: BIT_N_R(3, gb->cpu.hl.l);                         break;
    case 0x5e: BIT_N_INDIRECT_HL(3);                             break;
    case 0x5f: BIT_N_R(3, gb->cpu.af.a);                         break;
    case 0x60: BIT_N_R(4, gb->cpu.bc.b);                         break;
    case 0x61: BIT_N_R(4, gb->cpu.bc.c);                         break;
    case 0x62: BIT_N_R(4, gb->cpu.de.d);                         break;
    case 0x63: BIT_N_R(4, gb->cpu.de.e);                         break;
    case 0x64: BIT_N_R(4, gb->cpu.hl.h);                         break;
    case 0x65: BIT_N_R(4, gb->cpu.hl.l);                         break;
    case 0x66: BIT_N_INDIRECT_HL(4);                             break;
    case 0x67: BIT_N_R(4, gb->cpu.af.a);                         break;
    case 0x68: BIT_N_R(5, gb->cpu.bc.b);                         break;
    case 0x69: BIT_N_R(5, gb->cpu.bc.c);                         break;
    case 0x6a: BIT_N_R(5, gb->cpu.de.d);                         break;
    case 0x6b: BIT_N_R(5, gb->cpu.de.e);                         break;
    case 0x6c: BIT_N_R(5, gb->cpu.hl.h);                         break;
    case 0x6d: BIT_N_R(5, gb->cpu.hl.l);                         break;
    case 0x6e: BIT_N_INDIRECT_HL(5);                             break;
    case 0x6f: BIT_N_R(5, gb->cpu.af.a);                         break;
    case 0x70: BIT_N_R(6, gb->cpu.bc.b);                         break;
    case 0x71: BIT_N_R(6, gb->cpu.bc.c);                         break;
    case 0x72: BIT_N_R(6, gb->cpu.de.d);                         break;
    case 0x73: BIT_N_R(6, gb->cpu.de.e);                         break;
    case 0x74: BIT_N_R(6, gb->cpu.hl.h);                         break;
    case 0x75: BIT_N_R(6, gb->cpu.hl.l);                         break;
    case 0x76: BIT_N_INDIRECT_HL(6);                             break;
    case 0x77: BIT_N_R(6, gb->cpu.af.a);                         break;
    case 0x78: BIT_N_R(7, gb->cpu.bc.b);                         break;
    case 0x79: BIT_N_R(7, gb->cpu.bc.c);                         break;
    case 0x7a: BIT_N_R(7, gb->cpu.de.d);                         break;
    case 0x7b: BIT_N_R(7, gb->cpu.de.e);                         break;
    case 0x7c: BIT_N_R(7, gb->cpu.hl.h);                         break;
    case 0x7d: BIT_N_R(7, gb->cpu.hl.l);                         break;
    case 0x7e: BIT_N_INDIRECT_HL(7);                             break;
    case 0x7f: BIT_N_R(7, gb->cpu.af.a);                         break;
    case 0x80: RES_N_R(0, gb->cpu.bc.b);                         break;
    case 0x81: RES_N_R(0, gb->cpu.bc.c);                         break;
    case 0x82: RES_N_R(0, gb->cpu.de.d);                         break;
    case 0x83: RES_N_R(0, gb->cpu.de.e);                         break;
    case 0x84: RES_N_R(0, gb->cpu.hl.h);                         break;
    case 0x85: RES_N_R(0, gb->cpu.hl.l);                         break;
    case 0x86: RES_N_INDIRECT_HL(0);                             break;
    case 0x87: RES_N_R(0, gb->cpu.af.a);                         break;
    case 0x88: RES_N_R(1, gb->cpu.bc.b);                         break;
    case 0x89: RES_N_R(1, gb->cpu.bc.c);                         break;
    case 0x8a: RES_N_R(1, gb->cpu.de.d);                         break;
    case 0x8b: RES_N_R(1, gb->cpu.de.e);                         break;
    case 0x8c: RES_N_R(1, gb->cpu.hl.h);                         break;
    case 0x8d: RES_N_R(1, gb->cpu.hl.l);                         break;
    case 0x8e: RES_N_INDIRECT_HL(1);                             break;
    case 0x8f: RES_N_R(1, gb->cpu.af.a);                         break;
    case 0x90: RES_N_R(2, gb->cpu.bc.b);                         break;
    case 0x91: RES_N_R(2, gb->cpu.bc.c);                         break;
    case 0x92: RES_N_R(2, gb->cpu.de.d);                         break;
    case 0x93: RES_N_R(2, gb->cpu.de.e);                         break;
    case 0x94: RES_N_R(2, gb->cpu.hl.h);                         break;
    case 0x95: RES_N_R(2, gb->cpu.hl.l);                         break;
    case 0x96: RES_N_INDIRECT_HL(2);                             break;
    case 0x97: RES_N_R(2, gb->cpu.af.a);                         break;
    case 0x98: RES_N_R(3, gb->cpu.bc.b);                         break;
    case 0x99: RES_N_R(3, gb->cpu.bc.c);                         break;
    case 0x9a: RES_N_R(3, gb->cpu.de.d);                         break;
    case 0x9b: RES_N_R(3, gb->cpu.de.e);                         break;
    case 0x9c: RES_N_R(3, gb->cpu.hl.h);                         break;
    case 0x9d: RES_N_R(3, gb->cpu.hl.l);                         break;
    case 0x9e: RES_N_INDIRECT_HL(3);                             break;
    case 0x9f: RES_N_R(3, gb->cpu.af.a);                         break;
    case 0xa0: RES_N_R(4, gb->cpu.bc.b);                         break;
    case 0xa1: RES_N_R(4, gb->cpu.bc.c);                         break;
    case 0xa2: RES_N_R(4, gb->cpu.de.d);                         break;
    case 0xa3: RES_N_R(4, gb->cpu.de.e);                         break;
    case 0xa4: RES_N_R(4, gb->cpu.hl.h);                         break;
    case 0xa5: RES_N_R(4, gb->cpu.hl.l);                         break;
    case 0xa6: RES_N_INDIRECT_HL(4);                             break;
    case 0xa7: RES_N_R(4, gb->cpu.af.a);                         break;
    case 0xa8: RES_N_R(5, gb->cpu.bc.b);                         break;
    case 0xa9: RES_N_R(5, gb->cpu.bc.c);                         break;
    case 0xaa: RES_N_R(5, gb->cpu.de.d);                         break;
    case 0xab: RES_N_R(5, gb->cpu.de.e);                         break;
    case 0xac: RES_N_R(5, gb->cpu.hl.h);                         break;
    case 0xad: RES_N_R(5, gb->cpu.hl.l);                         break;
    case 0xae: RES_N_INDIRECT_HL(5);                             break;
    case 0xaf: RES_N_R(5, gb->cpu.af.a);                         break;
    case 0xb0: RES_N_R(6, gb->cpu.bc.b);                         break;
    case 0xb1: RES_N_R(6, gb->cpu.bc.c);                         break;
    case 0xb2: RES_N_R(6, gb->cpu.de.d);                         break;
    case 0xb3: RES_N_R(6, gb->cpu.de.e);                         break;
    case 0xb4: RES_N_R(6, gb->cpu.hl.h);                         break;
    case 0xb5: RES_N_R(6, gb->cpu.hl.l);                         break;
    case 0xb6: RES_N_INDIRECT_HL(6);                             break;
    case 0xb7: RES_N_R(6, gb->cpu.af.a);                         break;
    case 0xb8: RES_N_R(7, gb->cpu.bc.b);                         break;
    case 0xb9: RES_N_R(7, gb->cpu.bc.c);                         break;
    case 0xba: RES_N_R(7, gb->cpu.de.d);                         break;
    case 0xbb: RES_N_R(7, gb->cpu.de.e);                         break;
    case 0xbc: RES_N_R(7, gb->cpu.hl.h);                         break;
    case 0xbd: RES_N_R(7, gb->cpu.hl.l);                         break;
    case 0xbe: RES_N_INDIRECT_HL(7);                             break;
    case 0xbf: RES_N_R(7, gb->cpu.af.a);                         break;
    case 0xc0: SET_N_R(0, gb->cpu.bc.b);                         break;
    case 0xc1: SET_N_R(0, gb->cpu.bc.c);                         break;
    case 0xc2: SET_N_R(0, gb->cpu.de.d);                         break;
    case 0xc3: SET_N_R(0, gb->cpu.de.e);                         break;
    case 0xc4: SET_N_R(0, gb->cpu.hl.h);                         break;
    case 0xc5: SET_N_R(0, gb->cpu.hl.l);                         break;
    case 0xc6: SET_N_INDIRECT_HL(0);                             break;
    case 0xc7: SET_N_R(0, gb->cpu.af.a);                         break;
    case 0xc8: SET_N_R(1, gb->cpu.bc.b);                         break;
    case 0xc9: SET_N_R(1, gb->cpu.bc.c);                         break;
    case 0xca: SET_N_R(1, gb->cpu.de.d);                         break;
    case 0xcb: SET_N_R(1, gb->cpu.de.e);                         break;
    case 0xcc: SET_N_R(1, gb->cpu.hl.h);                         break;
    case 0xcd: SET_N_R(1, gb->cpu.hl.l);                         break;
    case 0xce: SET_N_INDIRECT_HL(1);                             break;
    case 0xcf: SET_N_R(1, gb->cpu.af.a);                         break;
    case 0xd0: SET_N_R(2, gb->cpu.bc.b);                         break;
    case 0xd1: SET_N_R(2, gb->cpu.bc.c);                         break;
    case 0xd2: SET_N_R(2, gb->cpu.de.d);                         break;
    case 0xd3: SET_N_R(2, gb->cpu.de.e);                         break;
    case 0xd4: SET_N_R(2, gb->cpu.hl.h);                         break;
    case 0xd5: SET_N_R(2, gb->cpu.hl.l);                         break;
    case 0xd6: SET_N_INDIRECT_HL(2);                             break;
    case 0xd7: SET_N_R(2, gb->cpu.af.a);                         break;
    case 0xd8: SET_N_R(3, gb->cpu.bc.b);                         break;
    case 0xd9: SET_N_R(3, gb->cpu.bc.c);                         break;
    case 0xda: SET_N_R(3, gb->cpu.de.d);                         break;
    case 0xdb: SET_N_R(3, gb->cpu.de.e);                         break;
    case 0xdc: SET_N_R(3, gb->cpu.hl.h);                         break;
    case 0xdd: SET_N_R(3, gb->cpu.hl.l);                         break;
    case 0xde: SET_N_INDIRECT_HL(3);                             break;
    case 0xdf: SET_N_R(3, gb->cpu.af.a);                         break;
    case 0xe0: SET_N_R(4, gb->cpu.bc.b);                         break;
    case 0xe1: SET_N_R(4, gb->cpu.bc.c);                         break;
    case 0xe2: SET_N_R(4, gb->cpu.de.d);                         break;
    case 0xe3: SET_N_R(4, gb->cpu.de.e);                         break;
    case 0xe4: SET_N_R(4, gb->cpu.hl.h);                         break;
    case 0xe5: SET_N_R(4, gb->cpu.hl.l);                         break;
    case 0xe6: SET_N_INDIRECT_HL(4);                             break;
    case 0xe7: SET_N_R(4, gb->cpu.af.a);                         break;
    case 0xe8: SET_N_R(5, gb->cpu.bc.b);                         break;
    case 0xe9: SET_N_R(5, gb->cpu.bc.c);                         break;
    case 0xea: SET_N_R(5, gb->cpu.de.d);                         break;
    case 0xeb: SET_N_R(5, gb->cpu.de.e);                         break;
    case 0xec: SET_N_R(5, gb->cpu.hl.h);                         break;
    case 0xed: SET_N_R(5, gb->cpu.hl.l);                         break;
    case 0xee: SET_N_INDIRECT_HL(5);                             break;
    case 0xef: SET_N_R(5, gb->cpu.af.a);                         break;
    case 0xf0: SET_N_R(6, gb->cpu.bc.b);                         break;
    case 0xf1: SET_N_R(6, gb->cpu.bc.c);                         break;
    case 0xf2: SET_N_R(6, gb->cpu.de.d);                         break;
    case 0xf3: SET_N_R(6, gb->cpu.de.e);                         break;
    case 0xf4: SET_N_R(6, gb->cpu.hl.h);                         break;
    case 0xf5: SET_N_R(6, gb->cpu.hl.l);                         break;
    case 0xf6: SET_N_INDIRECT_HL(6);                             break;
    case 0xf7: SET_N_R(6, gb->cpu.af.a);                         break;
    case 0xf8: SET_N_R(7, gb->cpu.bc.b);                         break;
    case 0xf9: SET_N_R(7, gb->cpu.bc.c);                         break;
    case 0xfa: SET_N_R(7, gb->cpu.de.d);                         break;
    case 0xfb: SET_N_R(7, gb->cpu.de.e);                         break;
    case 0xfc: SET_N_R(7, gb->cpu.hl.h);                         break;
    case 0xfd: SET_N_R(7, gb->cpu.hl.l);                         break;
    case 0xfe: SET_N_INDIRECT_HL(7);                             break;
    case 0xff: SET_N_R(7, gb->cpu.af.a);                         break;
    default:
        break;
    }
    return cbInstrCycle[opcode];
}

void cpu_step(struct gb *gb)
{
    uint8_t opcode, a, msb, lsb;
    uint16_t operand, res, carryPerBit;
    bool is_interrupt, statIntrLine;

    gb->executedCycle = 0;
    opcode = (gb->mode == HALT) ? 0x76 : CPU_FETCH_BYTE();
    gb->executedCycle += instrCycle[opcode] + ((gb->interrupt.interruptHandled) ? 5 : 0);
    switch (opcode) {
    case 0x00:                                                          break;
    case 0x01: gb->cpu.bc.val = sm83_fetch_word(gb);                    break;
    case 0x02: bus_write(gb, gb->cpu.bc.val, gb->cpu.af.a);             break;
    case 0x03: INC_RR(gb->cpu.bc.val);                                  break;
    case 0x04: INC_R(gb->cpu.bc.b);                                     break;
    case 0x05: DEC_R(gb->cpu.bc.b);                                     break;
    case 0x06: gb->cpu.bc.b = CPU_FETCH_BYTE();                         break;
    case 0x07: RLCA();                                                  break;
    case 0x08: 
        operand = sm83_fetch_word(gb);
        LD_INDIRECT_NN_SP(operand);
        break;
    case 0x09: ADD_HL_RR(gb->cpu.bc.val);                               break;
    case 0x0a: gb->cpu.af.a = bus_read(gb, gb->cpu.bc.val);             break;
    case 0x0b: DEC_RR(gb->cpu.bc.val);                                  break;
    case 0x0c: INC_R(gb->cpu.bc.c);                                     break;
    case 0x0d: DEC_R(gb->cpu.bc.c);                                     break;
    case 0x0e: gb->cpu.bc.c = CPU_FETCH_BYTE();                         break;
    case 0x0f: RRCA();                                                  break;
    case 0x10:                                                          break;
    case 0x11: gb->cpu.de.val = sm83_fetch_word(gb);                    break;
    case 0x12: bus_write(gb, gb->cpu.de.val, gb->cpu.af.a);             break;
    case 0x13: INC_RR(gb->cpu.de.val);                                  break;
    case 0x14: INC_R(gb->cpu.de.d);                                     break;
    case 0x15: DEC_R(gb->cpu.de.d);                                     break;
    case 0x16: gb->cpu.de.d = CPU_FETCH_BYTE();                         break;
    case 0x17: RLA();                                                   break;
    case 0x18: 
        operand = CPU_FETCH_BYTE();
        JP(gb->cpu.pc, operand, 1);
        break;
    case 0x19: ADD_HL_RR(gb->cpu.de.val);                               break;
    case 0x1a: gb->cpu.af.a = bus_read(gb, gb->cpu.de.val);             break;
    case 0x1b: DEC_RR(gb->cpu.de.val);                                  break;
    case 0x1c: INC_R(gb->cpu.de.e);                                     break;
    case 0x1d: DEC_R(gb->cpu.de.e);                                     break;
    case 0x1e: gb->cpu.de.e = CPU_FETCH_BYTE();                        break;
    case 0x1f: RRA();                                                   break;
    case 0x20: 
        operand = CPU_FETCH_BYTE();
        JP(gb->cpu.pc, operand, !gb->cpu.af.flag.z);
        break;
    case 0x21: gb->cpu.hl.val = sm83_fetch_word(gb);                    break;
    case 0x22: bus_write(gb, gb->cpu.hl.val++, gb->cpu.af.a);           break;
    case 0x23: INC_RR(gb->cpu.hl.val);                                  break;
    case 0x24: INC_R(gb->cpu.hl.h);                                     break;
    case 0x25: DEC_R(gb->cpu.hl.h);                                     break;
    case 0x26: gb->cpu.hl.h = CPU_FETCH_BYTE();                        break;
    case 0x27: DAA();                                                   break;
    case 0x28: 
        operand = CPU_FETCH_BYTE();
        JP(gb->cpu.pc, operand, gb->cpu.af.flag.z);
        break;
    case 0x29: ADD_HL_RR(gb->cpu.hl.val);                               break;
    case 0x2a: gb->cpu.af.a = bus_read(gb, gb->cpu.hl.val++);           break;
    case 0x2b: DEC_RR(gb->cpu.hl.val);                                  break;
    case 0x2c: INC_R(gb->cpu.hl.l);                                     break;
    case 0x2d: DEC_R(gb->cpu.hl.l);                                     break;
    case 0x2e: gb->cpu.hl.l = CPU_FETCH_BYTE();                        break;
    case 0x2f: CPL();                                                   break;
    case 0x30:
        operand = CPU_FETCH_BYTE();
        JP(gb->cpu.pc, operand, !gb->cpu.af.flag.c);
        break;
    case 0x31: gb->cpu.sp = sm83_fetch_word(gb);                        break;
    case 0x32: bus_write(gb, gb->cpu.hl.val--, gb->cpu.af.a);           break;
    case 0x33: INC_RR(gb->cpu.sp);                                      break;
    case 0x34: INC_INDIRECT_HL();                                       break;
    case 0x35: DEC_INDIRECT_HL();                                       break;
    case 0x36: 
        operand = CPU_FETCH_BYTE();
        LD_INDIRECT_HL_N(operand);
        break;
    case 0x37: SCF();                                                   break;
    case 0x38:
        operand = CPU_FETCH_BYTE();
        JP(gb->cpu.pc, operand, gb->cpu.af.flag.c);
        break;
    case 0x39: ADD_HL_RR(gb->cpu.sp);                                   break;
    case 0x3a: gb->cpu.af.a = bus_read(gb, gb->cpu.hl.val--);           break;
    case 0x3b: DEC_RR(gb->cpu.sp);                                      break;
    case 0x3c: INC_R(gb->cpu.af.a);                                     break;
    case 0x3d: DEC_R(gb->cpu.af.a);                                     break;
    case 0x3e: gb->cpu.af.a = CPU_FETCH_BYTE();                        break;
    case 0x3f: CCF();                                                   break;
    case 0x40: gb->cpu.bc.b = gb->cpu.bc.b;                             break;
    case 0x41: gb->cpu.bc.b = gb->cpu.bc.c;                             break;
    case 0x42: gb->cpu.bc.b = gb->cpu.de.d;                             break;
    case 0x43: gb->cpu.bc.b = gb->cpu.de.e;                             break;
    case 0x44: gb->cpu.bc.b = gb->cpu.hl.h;                             break;
    case 0x45: gb->cpu.bc.b = gb->cpu.hl.l;                             break;
    case 0x46: gb->cpu.bc.b = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x47: gb->cpu.bc.b = gb->cpu.af.a;                             break;
    case 0x48: gb->cpu.bc.c = gb->cpu.bc.b;                             break;
    case 0x49: gb->cpu.bc.c = gb->cpu.bc.c;                             break;
    case 0x4a: gb->cpu.bc.c = gb->cpu.de.d;                             break;
    case 0x4b: gb->cpu.bc.c = gb->cpu.de.e;                             break;
    case 0x4c: gb->cpu.bc.c = gb->cpu.hl.h;                             break;
    case 0x4d: gb->cpu.bc.c = gb->cpu.hl.l;                             break;
    case 0x4e: gb->cpu.bc.c = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x4f: gb->cpu.bc.c = gb->cpu.af.a;                             break;
    case 0x50: gb->cpu.de.d = gb->cpu.bc.b;                             break;
    case 0x51: gb->cpu.de.d = gb->cpu.bc.c;                             break;
    case 0x52: gb->cpu.de.d = gb->cpu.de.d;                             break;
    case 0x53: gb->cpu.de.d = gb->cpu.de.e;                             break;
    case 0x54: gb->cpu.de.d = gb->cpu.hl.h;                             break;
    case 0x55: gb->cpu.de.d = gb->cpu.hl.l;                             break;
    case 0x56: gb->cpu.de.d = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x57: gb->cpu.de.d = gb->cpu.af.a;                             break;
    case 0x58: gb->cpu.de.e = gb->cpu.bc.b;                             break;
    case 0x59: gb->cpu.de.e = gb->cpu.bc.c;                             break;
    case 0x5a: gb->cpu.de.e = gb->cpu.de.d;                             break;
    case 0x5b: gb->cpu.de.e = gb->cpu.de.e;                             break;
    case 0x5c: gb->cpu.de.e = gb->cpu.hl.h;                             break;
    case 0x5d: gb->cpu.de.e = gb->cpu.hl.l;                             break;
    case 0x5e: gb->cpu.de.e = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x5f: gb->cpu.de.e = gb->cpu.af.a;                             break;
    case 0x60: gb->cpu.hl.h = gb->cpu.bc.b;                             break;
    case 0x61: gb->cpu.hl.h = gb->cpu.bc.c;                             break;
    case 0x62: gb->cpu.hl.h = gb->cpu.de.d;                             break;
    case 0x63: gb->cpu.hl.h = gb->cpu.de.e;                             break;
    case 0x64: gb->cpu.hl.h = gb->cpu.hl.h;                             break;
    case 0x65: gb->cpu.hl.h = gb->cpu.hl.l;                             break;
    case 0x66: gb->cpu.hl.h = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x67: gb->cpu.hl.h = gb->cpu.af.a;                             break;
    case 0x68: gb->cpu.hl.l = gb->cpu.bc.b;                             break;
    case 0x69: gb->cpu.hl.l = gb->cpu.bc.c;                             break;
    case 0x6a: gb->cpu.hl.l = gb->cpu.de.d;                             break;
    case 0x6b: gb->cpu.hl.l = gb->cpu.de.e;                             break;
    case 0x6c: gb->cpu.hl.l = gb->cpu.hl.h;                             break;
    case 0x6d: gb->cpu.hl.l = gb->cpu.hl.l;                             break;
    case 0x6e: gb->cpu.hl.l = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x6f: gb->cpu.hl.l = gb->cpu.af.a;                             break;
    case 0x70: bus_write(gb, gb->cpu.hl.val, gb->cpu.bc.b);             break;
    case 0x71: bus_write(gb, gb->cpu.hl.val, gb->cpu.bc.c);             break;
    case 0x72: bus_write(gb, gb->cpu.hl.val, gb->cpu.de.d);             break;
    case 0x73: bus_write(gb, gb->cpu.hl.val, gb->cpu.de.e);             break;
    case 0x74: bus_write(gb, gb->cpu.hl.val, gb->cpu.hl.h);             break;
    case 0x75: bus_write(gb, gb->cpu.hl.val, gb->cpu.hl.l);             break;
    case 0x76: HALT();                                                  break;
    case 0x77: bus_write(gb, gb->cpu.hl.val, gb->cpu.af.a);             break;
    case 0x78: gb->cpu.af.a = gb->cpu.bc.b;                             break;
    case 0x79: gb->cpu.af.a = gb->cpu.bc.c;                             break;
    case 0x7a: gb->cpu.af.a = gb->cpu.de.d;                             break;
    case 0x7b: gb->cpu.af.a = gb->cpu.de.e;                             break;
    case 0x7c: gb->cpu.af.a = gb->cpu.hl.h;                             break;
    case 0x7d: gb->cpu.af.a = gb->cpu.hl.l;                             break;
    case 0x7e: gb->cpu.af.a = bus_read(gb, gb->cpu.hl.val);             break;
    case 0x7f: gb->cpu.af.a = gb->cpu.af.a;                             break;
    case 0x80: ADD(gb->cpu.bc.b, 0);                                    break;
    case 0x81: ADD(gb->cpu.bc.c, 0);                                    break;
    case 0x82: ADD(gb->cpu.de.d, 0);                                    break;
    case 0x83: ADD(gb->cpu.de.e, 0);                                    break;
    case 0x84: ADD(gb->cpu.hl.h, 0);                                    break;
    case 0x85: ADD(gb->cpu.hl.l, 0);                                    break;
    case 0x86: 
        operand = bus_read(gb, gb->cpu.hl.val);
        ADD(operand, 0);
        break;
    case 0x87: ADD(gb->cpu.af.a, 0);                                    break;
    case 0x88: ADD(gb->cpu.bc.b, gb->cpu.af.flag.c);                    break;
    case 0x89: ADD(gb->cpu.bc.c, gb->cpu.af.flag.c);                    break;
    case 0x8a: ADD(gb->cpu.de.d, gb->cpu.af.flag.c);                    break;
    case 0x8b: ADD(gb->cpu.de.e, gb->cpu.af.flag.c);                    break;
    case 0x8c: ADD(gb->cpu.hl.h, gb->cpu.af.flag.c);                    break;
    case 0x8d: ADD(gb->cpu.hl.l, gb->cpu.af.flag.c);                    break;
    case 0x8e: 
        operand = bus_read(gb, gb->cpu.hl.val);
        ADD(operand, gb->cpu.af.flag.c);
        break;
    case 0x8f: ADD(gb->cpu.af.a, gb->cpu.af.flag.c);                    break;
    case 0x90: SUB(gb->cpu.bc.b, 0);                                    break;
    case 0x91: SUB(gb->cpu.bc.c, 0);                                    break;
    case 0x92: SUB(gb->cpu.de.d, 0);                                    break;
    case 0x93: SUB(gb->cpu.de.e, 0);                                    break;
    case 0x94: SUB(gb->cpu.hl.h, 0);                                    break;
    case 0x95: SUB(gb->cpu.hl.l, 0);                                    break;
    case 0x96:
        operand = bus_read(gb, gb->cpu.hl.val);
        SUB(operand, 0);      
        break;
    case 0x97: SUB(gb->cpu.af.a, 0);                                    break;
    case 0x98: SUB(gb->cpu.bc.b, gb->cpu.af.flag.c);                    break;
    case 0x99: SUB(gb->cpu.bc.c, gb->cpu.af.flag.c);                    break;
    case 0x9a: SUB(gb->cpu.de.d, gb->cpu.af.flag.c);                    break;
    case 0x9b: SUB(gb->cpu.de.e, gb->cpu.af.flag.c);                    break;
    case 0x9c: SUB(gb->cpu.hl.h, gb->cpu.af.flag.c);                    break;
    case 0x9d: SUB(gb->cpu.hl.l, gb->cpu.af.flag.c);                    break;
    case 0x9e:
        operand = bus_read(gb, gb->cpu.hl.val);
        SUB(operand, gb->cpu.af.flag.c);
        break;
    case 0x9f: SUB(gb->cpu.af.a, gb->cpu.af.flag.c);                    break;
    case 0xa0: AND(gb->cpu.bc.b);                                       break;
    case 0xa1: AND(gb->cpu.bc.c);                                       break;
    case 0xa2: AND(gb->cpu.de.d);                                       break;
    case 0xa3: AND(gb->cpu.de.e);                                       break;
    case 0xa4: AND(gb->cpu.hl.h);                                       break;
    case 0xa5: AND(gb->cpu.hl.l);                                       break;
    case 0xa6: 
        operand = bus_read(gb, gb->cpu.hl.val);
        AND(operand);
        break;
    case 0xa7: AND(gb->cpu.af.a);                                       break;
    case 0xa8: XOR(gb->cpu.bc.b);                                       break;
    case 0xa9: XOR(gb->cpu.bc.c);                                       break;
    case 0xaa: XOR(gb->cpu.de.d);                                       break;
    case 0xab: XOR(gb->cpu.de.e);                                       break;
    case 0xac: XOR(gb->cpu.hl.h);                                       break;
    case 0xad: XOR(gb->cpu.hl.l);                                       break;
    case 0xae: 
        operand = bus_read(gb, gb->cpu.hl.val);
        XOR(operand);
        break;
    case 0xaf: XOR(gb->cpu.af.a);                                       break;
    case 0xb0: OR(gb->cpu.bc.b);                                        break;
    case 0xb1: OR(gb->cpu.bc.c);                                        break;
    case 0xb2: OR(gb->cpu.de.d);                                        break;
    case 0xb3: OR(gb->cpu.de.e);                                        break;
    case 0xb4: OR(gb->cpu.hl.h);                                        break;
    case 0xb5: OR(gb->cpu.hl.l);                                        break;
    case 0xb6: 
        operand = bus_read(gb, gb->cpu.hl.val);
        OR(operand);
        break;
    case 0xb7: OR(gb->cpu.af.a);                                        break;
    case 0xb8: CP(gb->cpu.bc.b);                                        break;
    case 0xb9: CP(gb->cpu.bc.c);                                        break;
    case 0xba: CP(gb->cpu.de.d);                                        break;
    case 0xbb: CP(gb->cpu.de.e);                                        break;
    case 0xbc: CP(gb->cpu.hl.h);                                        break;
    case 0xbd: CP(gb->cpu.hl.l);                                        break;
    case 0xbe:
        operand = bus_read(gb, gb->cpu.hl.val);
        CP(operand);
        break;
    case 0xbf: CP(gb->cpu.af.a);                                        break;
    case 0xc0: RET(opcode, !gb->cpu.af.flag.z);                         break;
    case 0xc1: gb->cpu.bc.val = cpu_pop_word(gb);                      break;
    case 0xc2: 
        operand = sm83_fetch_word(gb);
        JP(operand, 0, !gb->cpu.af.flag.z);
        break;
    case 0xc3: 
        operand = sm83_fetch_word(gb);
        JP(operand, 0, 1);
        break; 
    case 0xc4:
        operand = sm83_fetch_word(gb);
        CALL(operand, !gb->cpu.af.flag.z);
        break;
    case 0xc5: PUSH_RR(gb->cpu.bc.val);                                 break;
    case 0xc6: 
        operand = CPU_FETCH_BYTE();
        ADD(operand, 0);
        break;
    case 0xc7: RST_N(0x00);                                             break;
    case 0xc8: RET(opcode, gb->cpu.af.flag.z);                          break;
    case 0xc9: RET(opcode, 1);                                          break;
    case 0xca: 
        operand = sm83_fetch_word(gb);
        JP(operand, 0, gb->cpu.af.flag.z);
        break;
    case 0xcb:
        operand = CPU_FETCH_BYTE();
        gb->executedCycle += execute_cb_instructions(gb, operand);
        break;
    case 0xcc:
        operand = sm83_fetch_word(gb);
        CALL(operand, gb->cpu.af.flag.z);
        break;
    case 0xcd: 
        operand = sm83_fetch_word(gb);
        CALL(operand, 1);
        break;
    case 0xce: 
        operand = CPU_FETCH_BYTE();
        ADD(operand, gb->cpu.af.flag.c);
        break;
    case 0xcf: RST_N(0x08);                                             break;
    case 0xd0: RET(opcode, !gb->cpu.af.flag.c);                         break;
    case 0xd1: gb->cpu.de.val = cpu_pop_word(gb);                      break;
    case 0xd2: 
        operand = sm83_fetch_word(gb);
        JP(operand, 0, !gb->cpu.af.flag.c);
        break;
    case 0xd4: 
        operand = sm83_fetch_word(gb);
        CALL(operand, !gb->cpu.af.flag.c);
        break;
    case 0xd5: PUSH_RR(gb->cpu.de.val);                                 break;
    case 0xd6:
        operand = CPU_FETCH_BYTE();
        SUB(operand, 0);
        break;
    case 0xd7: RST_N(0x10);                                             break;
    case 0xd8: RET(opcode, gb->cpu.af.flag.c);                          break;
    case 0xd9: RETI();                                                  break;
    case 0xda:
        operand = sm83_fetch_word(gb);
        JP(operand, 0, gb->cpu.af.flag.c);
        break;
    case 0xdc: 
        operand = sm83_fetch_word(gb);
        CALL(operand, gb->cpu.af.flag.c);
        break;
    case 0xde:
        operand = CPU_FETCH_BYTE();
        SUB(operand, gb->cpu.af.flag.c);
        break;
    case 0xdf: RST_N(0x18);                                             break;
    case 0xe0: 
        operand = CPU_FETCH_BYTE();
        LDH_INDIRECT_N_A(operand);
        break;
    case 0xe1: gb->cpu.hl.val = cpu_pop_word(gb);                      break;
    case 0xe2: LDH_INDIRECT_C_A();                                      break;
    case 0xe5: PUSH_RR(gb->cpu.hl.val);                                 break;
    case 0xe6: 
        operand = CPU_FETCH_BYTE();
        AND(operand);
        break;
    case 0xe7: RST_N(0x20);                                             break;
    case 0xe8: 
        operand = CPU_FETCH_BYTE();
        ADD_SP_I8(operand);
        break;
    case 0xe9: gb->cpu.pc = gb->cpu.hl.val;                             break;
    case 0xea:
        operand = sm83_fetch_word(gb);
        bus_write(gb, operand, gb->cpu.af.a);
        break;
    case 0xee: 
        operand = CPU_FETCH_BYTE();
        XOR(operand);
        break;
    case 0xef: RST_N(0x28);                                             break;
    case 0xf0: 
        operand = CPU_FETCH_BYTE();
        LDH_A_INDIRECT_N(operand);
        break;
    case 0xf1: 
        operand = cpu_pop_word(gb);
        gb->cpu.af.val = (operand & 0xfff0) & ~0x000f;
        break;
    case 0xf2: LDH_A_INDIRECT_C();                                      break;
    case 0xf3: DI();                                                    break;
    case 0xf5: PUSH_RR(gb->cpu.af.val);                                 break;
    case 0xf6:
        operand = CPU_FETCH_BYTE();
        OR(operand);
        break;
    case 0xf7: RST_N(0x30);                                             break;
    case 0xf8: 
        operand = CPU_FETCH_BYTE();
        LD_HL_SP_PLUS_I8(operand);
        break;
    case 0xf9: gb->cpu.sp = gb->cpu.hl.val;                             break;
    case 0xfa: 
        operand = sm83_fetch_word(gb);
        gb->cpu.af.a = bus_read(gb, operand);
        break;
    case 0xfb: EI();                                                    break;
    case 0xfe: 
        operand = CPU_FETCH_BYTE();
        CP(operand);
        break;
    case 0xff: RST_N(0x38);                                             break;
    default:
        printf("Unknown opcode 0x%02x\n", opcode);
        break;
    }

    /* timer handling */
    gb->timer.div += gb->executedCycle;
    if ((gb->timer.div >= timerClockFrequency[gb->timer.tac.freq]) && (gb->timer.tac.enable)) {
        gb->timer.div -= timerClockFrequency[gb->timer.tac.freq];
        if (gb->timer.tima == 0xff) {
            INTERRUPT_REQUEST(INTERRUPT_SRC_TIMER);
        }
        gb->timer.tima = (gb->timer.tima == 0xff) ? gb->timer.tma : gb->timer.tima + 1;
    }

    /* oam dma handling */
    if (gb->dma.mode != OFF) {
        gb->dma.tick += gb->executedCycle;
        if (gb->dma.tick >= 1 && gb->dma.mode == WAITING) {
            gb->dma.tick -= 1;
            gb->dma.mode = TRANSFERING;
        } else if (gb->dma.tick >= 160 && gb->dma.mode == TRANSFERING) {
            for (int i = 0; i < 160; i++) {
                uint8_t transfer_val = bus_read(gb, gb->dma.startAddr + i);
                gb->oam[i] = transfer_val;
            }
            gb->dma.mode = OFF;
        }
    }

    /* ppu handling */
    if (!gb->ppu.lcdc.ppuEnable) {
        return;
    }
    gb->ppu.ticks += gb->executedCycle * 4;
    if (gb->ppu.ly <= 143) {
        if (gb->ppu.ticks <= 80 && gb->ppu.mode != OAM_SCAN) {
            SET_MODE(OAM_SCAN);
        } else if (gb->ppu.ticks <= 252 && gb->ppu.mode != DRAWING) {
            SET_MODE(DRAWING);
        } else if (gb->ppu.ticks <= 456 && gb->ppu.mode != HBLANK) {
            SET_MODE(HBLANK);
        }
    }
    if (gb->ppu.ticks > 456) {
        gb->ppu.scanLineReady = true;
        gb->ppu.ticks -= 456;
        if (gb->ppu.ly <= 143) {
            // ppu_oam_scan
            if (gb->ppu.lcdc.objEnable) {
                for (int i = 0; i < 40; i++) {
                    if (gb->oam[i * 4 + 1] > 0 && gb->ppu.oamEntryCounter < 10 &&
                        IN_RANGE(gb->ppu.ly, gb->oam[i * 4] - 16, gb->oam[i * 4] - 17 + gb->ppu.lcdc.objSize)) {
                        gb->ppu.oamEntry[gb->ppu.oamEntryCounter].y = gb->oam[i * 4];
                        gb->ppu.oamEntry[gb->ppu.oamEntryCounter].x = gb->oam[i * 4 + 1];
                        gb->ppu.oamEntry[gb->ppu.oamEntryCounter].tileIndex = gb->oam[i * 4 + 2];
                        gb->ppu.oamEntry[gb->ppu.oamEntryCounter].attributes.val = gb->oam[i * 4 + 3];
                        gb->ppu.oamEntryCounter++;
                    }
                    if (gb->ppu.oamEntryCounter == 10)
                        break;
                }
            }
            qsort(gb->ppu.oamEntry, gb->ppu.oamEntryCounter, sizeof(struct oam_entry), cmpfunc);

            // draw the scanline
            ppu_draw_scanline(gb);
            SET_MODE(HBLANK);
        }

        // hblank & vblank handler
        if (gb->ppu.mode == HBLANK) {
            gb->ppu.ly++;
            gb->ppu.stat.lycEqualLy = gb->ppu.ly == gb->ppu.lyc;
            if (gb->ppu.ly == 144) {
                SET_MODE(VBLANK);
                if (gb->ppu.lcdc.ppuEnable) {
                    INTERRUPT_REQUEST(INTERRUPT_SRC_VBLANK);
                }
                gb->ppu.frameReady = true;
                gb->ppu.windowLineCounter = 0;
                gb->ppu.drawWindowThisLine = false;
                gb->ppu.windowInFrame = false;
            } else {
                if (gb->ppu.wy == gb->ppu.ly && !gb->ppu.windowInFrame)
                    gb->ppu.windowInFrame = true;
                SET_MODE(OAM_SCAN);
                if (gb->ppu.drawWindowThisLine) {
                    gb->ppu.windowLineCounter++;
                    gb->ppu.drawWindowThisLine = false;
                }
            }
            gb->ppu.ticks = 0;
            gb->ppu.oamEntryCounter = 0;
        } else if (gb->ppu.mode == VBLANK) {
            if (gb->ppu.ly == 153) {
                gb->ppu.ly = 0;
                if (gb->ppu.wy == gb->ppu.ly && !gb->ppu.windowInFrame)
                    gb->ppu.windowInFrame = true;
                gb->ppu.oamEntryCounter = 0;
                SET_MODE(OAM_SCAN);
            } else {
                gb->ppu.ly++;
            }
            gb->ppu.stat.lycEqualLy = gb->ppu.ly == gb->ppu.lyc;
            gb->ppu.ticks = 0;
        }
        gb->ppu.scanLineReady = true;
    }
    if (gb->interrupt.ie & INTERRUPT_SRC_LCD) {
        statIntrLine = ((gb->ppu.stat.mode0IntrSelect && gb->ppu.stat.ppuMode == HBLANK) ||
        (gb->ppu.stat.mode1IntrSelect && gb->ppu.stat.ppuMode == VBLANK) ||
        (gb->ppu.stat.mode2IntrSelect && gb->ppu.stat.ppuMode == OAM_SCAN) || 
        (gb->ppu.stat.lycIntrSelect && gb->ppu.stat.lycEqualLy));
        if (!gb->ppu.statIntrLine && statIntrLine)
            INTERRUPT_REQUEST(INTERRUPT_SRC_LCD);
        gb->ppu.statIntrLine = statIntrLine;
    }

    interrupt_process(gb);
}

/**********************************************************************************************/
/************************************* PPU related parts **************************************/
/**********************************************************************************************/

#define GET_COLOR(lo, hi)        \
    ((lo) | ((hi) << 1))
#define GET_COLOR_ID(which_pal, color_id)       \
    (gb->ppu.pal[which_pal] >> ((color_id) * 2)) & 0x03
#define READ_vRAM(addr)         \
    gb->vRAM[addr - 0x8000]

int cmpfunc(const void *a, const void *b)
{
    struct oam_entry *sa = (struct oam_entry *)a;
    struct oam_entry *sb = (struct oam_entry *)b;

    return (sa->x - sb->x);
}

void ppu_draw_scanline(struct gb *gb)
{
    uint8_t tileIndex, colorIDLow, colorIDHigh, offsetX, offsetY, xPos, yPos, 
            windowOffset, nonWindowRange, colorID[SCREEN_WIDTH], color[SCREEN_WIDTH] = {0};
    uint16_t tile_map_addr, tile_addr;
    bool pixel_type[SCREEN_WIDTH] = {0};

    // if LCDC bit 0 is disabled, no rendering bg and window
    if (!gb->ppu.lcdc.bgWinEnable)
        goto sprite;

    // we deal with window first
    windowOffset = 0;
    gb->ppu.drawWindowThisLine = gb->ppu.lcdc.winEnable && gb->ppu.windowInFrame && (IN_RANGE(gb->ppu.wx - 7, -6, 159));
    if (gb->ppu.drawWindowThisLine) {
        for (int i = gb->ppu.wx - 7; i < SCREEN_WIDTH; i++) {
            if (i < 0)
                continue;
            if ((windowOffset % 8) == 0) {
                tileIndex = bus_read(gb, gb->ppu.lcdc.winTileMap + ((windowOffset / 8 + 32 * (gb->ppu.windowLineCounter / 8)) & 0x3ff));
                tile_addr = (gb->ppu.lcdc.bgWinTiles == 0x8000) ? 0x8000 + 16 * (uint8_t)tileIndex : 0x8800 + 16 * ((tileIndex + 0x80) % 0x100);
            }
            colorIDLow = (bus_read(gb, tile_addr + (gb->ppu.windowLineCounter % 8) * 2) >> (7 - (windowOffset % 8))) & 0x01;
            colorIDHigh = (bus_read(gb, tile_addr + (gb->ppu.windowLineCounter % 8) * 2 + 1) >> (7 - (windowOffset % 8))) & 0x01;
            colorID[i] = colorIDLow | (colorIDHigh << 1);
            color[i] = GET_COLOR_ID(BGP, colorIDLow | (colorIDHigh << 1));
            // gb->backBufferPtr[i + gb->ppu.ly * LCD_HEIGHT] = ili9225Palette[GET_COLOR_ID(BGP, colorID[i])];
            gb->backBufferPtr[i + gb->ppu.ly * SCREEN_WIDTH] = ili9225Palette[GET_COLOR_ID(BGP, colorID[i])];
            windowOffset++;
        }
        windowOffset = 0;
    }

    // then come to background
    nonWindowRange = (gb->ppu.drawWindowThisLine) ? gb->ppu.wx - 7 : SCREEN_WIDTH;
    if (gb->ppu.drawWindowThisLine) {
        nonWindowRange = ((gb->ppu.wx - 7) < 0) ? 0 : gb->ppu.wx - 7;
    } else {
        nonWindowRange = SCREEN_WIDTH;
    }
    offsetY = (gb->ppu.ly + gb->ppu.scy) & 0xff;
    for (int i = 0; i < nonWindowRange; i++) {
        offsetX = (i + gb->ppu.scx) & 0xff;
        tileIndex = bus_read(gb, gb->ppu.lcdc.bgTileMap + ((offsetX / 8 + 32 * (offsetY / 8)) & 0x3ff));
        tile_addr = (gb->ppu.lcdc.bgWinTiles == 0x8000)
                        ? 0x8000 + 16 * (uint8_t)tileIndex : 0x8800 + 16 * ((tileIndex + 0x80) % 0x100);
        colorIDLow = (bus_read(gb, tile_addr + (offsetY % 8) * 2) >> (7 - (offsetX % 8))) & 0x01;
        colorIDHigh = (bus_read(gb, tile_addr + (offsetY % 8) * 2 + 1) >> (7 - (offsetX % 8))) & 0x01;
        colorID[i] = colorIDLow | (colorIDHigh << 1);
        color[i] = GET_COLOR_ID(BGP, colorID[i]);
        // gb->backBufferPtr[i + gb->ppu.ly * LCD_HEIGHT] = ili9225Palette[GET_COLOR_ID(BGP, colorID[i])];
        gb->backBufferPtr[i + gb->ppu.ly * SCREEN_WIDTH] = ili9225Palette[GET_COLOR_ID(BGP, colorID[i])];
    }

sprite:
    // finally, sprite
    if (!gb->ppu.lcdc.objEnable || !gb->ppu.oamEntryCounter)
        return;
    for (int i = gb->ppu.oamEntryCounter - 1; i >= 0; i--) {
        tileIndex = gb->ppu.oamEntry[i].tileIndex;
        yPos = (gb->ppu.ly - (gb->ppu.oamEntry[i].y - 16)) % 16;
        offsetY = (!gb->ppu.oamEntry[i].attributes.yFlip) ? yPos : gb->ppu.lcdc.objSize - 1 - (yPos);
        if (gb->ppu.lcdc.objSize == 16 && yPos >= 8)  // bottom
            tileIndex = (gb->ppu.oamEntry[i].attributes.yFlip) ?  tileIndex & 0xfe : tileIndex | 0x01;
        else if (gb->ppu.lcdc.objSize == 16 && yPos <= 7) // top
            tileIndex = (gb->ppu.oamEntry[i].attributes.yFlip) ?  tileIndex | 0x01 : tileIndex & 0xfe;
        tile_addr = 0x8000 + 16 * (uint8_t)tileIndex;
        uint8_t test = (!gb->ppu.oamEntry[i].attributes.yFlip) ? (yPos % 8) : 7 - (yPos % 8);
        for (int j = gb->ppu.oamEntry[i].x - 8; j <= gb->ppu.oamEntry[i].x; j++) {
            if (j > 159 || j < 0)
                continue;
            offsetX = (gb->ppu.oamEntry[i].attributes.xFlip) ? (j - (gb->ppu.oamEntry[i].x - 8)) : 7 - (j - (gb->ppu.oamEntry[i].x - 8));
            colorIDLow = (READ_vRAM(tile_addr + (test) * 2) >> (offsetX)) & 0x01;
            colorIDHigh = (READ_vRAM(tile_addr + (test) * 2 + 1) >> (offsetX)) & 0x01;
            if (((pixel_type[j] == BG_WIN) && (!GET_COLOR(colorIDLow, colorIDHigh) || (GET_COLOR(colorIDLow, colorIDHigh) > 0 && 
                gb->ppu.oamEntry[i].attributes.priority && colorID[j] > 0))) ||
                ((pixel_type[j] == SPRITE) && (colorID[j] > 0 && !GET_COLOR(colorIDLow, colorIDHigh))))
                continue;
            color[j] = GET_COLOR_ID(gb->ppu.oamEntry[i].attributes.dmgPalette, GET_COLOR(colorIDLow, colorIDHigh));
            // gb->backBufferPtr[j + gb->ppu.ly * LCD_HEIGHT] = ili9225Palette[GET_COLOR_ID(gb->ppu.oamEntry[i].attributes.dmgPalette, GET_COLOR(colorIDLow, colorIDHigh))];
            gb->backBufferPtr[j + gb->ppu.ly * SCREEN_WIDTH] = ili9225Palette[GET_COLOR_ID(gb->ppu.oamEntry[i].attributes.dmgPalette, GET_COLOR(colorIDLow, colorIDHigh))];
            colorID[j] = GET_COLOR(colorIDLow, colorIDHigh);
            pixel_type[j] = SPRITE;
        }
    }
}

void ppu_check_stat_intr(struct gb *gb)
{
    bool statIntrLine = 0;

    statIntrLine = ((gb->ppu.stat.mode0IntrSelect && gb->ppu.stat.ppuMode == HBLANK) ||
        (gb->ppu.stat.mode1IntrSelect && gb->ppu.stat.ppuMode == VBLANK) ||
        (gb->ppu.stat.mode2IntrSelect && gb->ppu.stat.ppuMode == OAM_SCAN) || 
        (gb->ppu.stat.lycIntrSelect && gb->ppu.stat.lycEqualLy));
    if (!gb->ppu.statIntrLine && statIntrLine) {
        INTERRUPT_REQUEST(INTERRUPT_SRC_LCD);
    }
    gb->ppu.statIntrLine = statIntrLine;
}

void interrupt_process(struct gb *gb)
{
    bool is_interrupt = IS_INTERRUPT_PENDING();

    if (gb->mode == HALT)
        gb->mode = (is_interrupt) ? NORMAL : HALT;
    gb->interrupt.interruptHandled = gb->cpu.ime && is_interrupt;
    if (gb->interrupt.interruptHandled) {
        if ((gb->interrupt.ie & gb->interrupt.flag & INTERRUPT_SRC_VBLANK) == INTERRUPT_SRC_VBLANK) {
            gb->cpu.ime = false;
            cpu_push_word(gb, gb->cpu.pc);
            gb->interrupt.flag &= ~INTERRUPT_SRC_VBLANK;
            gb->cpu.pc = 0x40;
        } else if ((gb->interrupt.ie & gb->interrupt.flag & INTERRUPT_SRC_LCD) == INTERRUPT_SRC_LCD) {
            gb->cpu.ime = false;
            cpu_push_word(gb, gb->cpu.pc);
            gb->interrupt.flag &= ~INTERRUPT_SRC_LCD;
            gb->cpu.pc = 0x48;
        } else if ((gb->interrupt.ie & gb->interrupt.flag & INTERRUPT_SRC_TIMER) == INTERRUPT_SRC_TIMER) {
            gb->cpu.ime = false;
            cpu_push_word(gb, gb->cpu.pc);
            gb->interrupt.flag &= ~INTERRUPT_SRC_TIMER;
            gb->cpu.pc = 0x50;
        } else if ((gb->interrupt.ie & gb->interrupt.flag & INTERRUPT_SRC_SERIAL) == INTERRUPT_SRC_SERIAL) {
            gb->cpu.ime = false;
            cpu_push_word(gb, gb->cpu.pc);
            gb->interrupt.flag &= ~INTERRUPT_SRC_SERIAL;
            gb->cpu.pc = 0x58;
        } else if ((gb->interrupt.ie & gb->interrupt.flag & INTERRUPT_SRC_JOYPAD) == INTERRUPT_SRC_JOYPAD) {
            gb->cpu.ime = false;
            cpu_push_word(gb, gb->cpu.pc);
            gb->interrupt.flag &= ~INTERRUPT_SRC_JOYPAD;
            gb->cpu.pc = 0x60;
        }
    }
}


/**********************************************************************************************/
/************************************* timer related parts ************************************/
/**********************************************************************************************/

/**********************************************************************************************/
/********************************** cartridge related parts ***********************************/
/**********************************************************************************************/

const char *ROMType[] = {
	[0x00] = "ROM ONLY",
	[0x01] = "MBC1",
	[0x02] = "MBC1+RAM",
	[0x03] = "MBC1+RAM+BATTERY",
	[0x05] = "MBC2",
	[0x06] = "MBC2+BATTERY",
	[0x08] = "ROM+RAM",
	[0x09] = "ROM+RAM+BATTERY",
	[0x0B] = "MMM01",
	[0x0C] = "MMM01+RAM",
	[0x0D] = "MMM01+RAM+BATTERY",
	[0x0F] = "MBC3+TIMER+BATTERY",
	[0x10] = "MBC3+TIMER+RAM+BATTERY",
	[0x11] = "MBC3",
	[0x12] = "MBC3+RAM",
	[0x13] = "MBC3+RAM+BATTERY",
	[0x15] = "MBC4",
	[0x16] = "MBC4+RAM",
	[0x17] = "MBC4+RAM+BATTERY",
	[0x19] = "MBC5",
	[0x1A] = "MBC5+RAM",
	[0x1B] = "MBC5+RAM+BATTERY",
	[0x1C] = "MBC5+RUMBLE",
	[0x1D] = "MBC5+RUMBLE+RAM",
	[0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
	[0xFC] = "POCKET CAMERA",
	[0xFD] = "BANDAI TAMA5",
	[0xFE] = "HuC3",
	[0xFF] = "HuC1+RAM+BATTERY",
};

const char *ROMSize[12] = {
    "32 KiB",
    "64 KiB",
    "128 KiB",
    "256 KiB",
    "512 KiB",
    "1 MiB",
    "2 MiB",
    "4 MiB",
    "8 MiB",
    "1.1 MiB",
    "1.2 MiB",
    "1.5 MiB",
};

const char *SRAMSize[6] = {
    "0",
    "0",
    "8 KiB",
    "32 KiB",
    "128 KiB",
    "64 KiB",
};

static int SRAMSizeInNumber[] = {
    0, 0, 8 * KiB, 32 * KiB, 128 * KiB, 64 * KiB
};

uint8_t mbc1_read(struct gb *gb, uint16_t addr)
{
    if (IN_RANGE(addr, 0x0000, 0x3fff))
        return gb->cart.rom.data[addr];
    else if (IN_RANGE(addr, 0x4000, 0x7fff))
        return gb->cart.rom.data[addr - 0x4000 + 0x4000 * (gb->mbc.mbc1.romBank & mbc1BitMask[gb->cart.rom.bankNumber])];
    else if (IN_RANGE(addr, 0xa000, 0xbfff)) {
        if (gb->mbc.mbc1.ramEnable)
            return gb->cart.ram.data[addr - 0xa000 + 0x2000 * gb->mbc.mbc1.ramBank];    
    }
    return 0xff;
}

void mbc1_write(struct gb *gb, uint16_t addr, uint8_t val)
{
    if (IN_RANGE(addr, 0x0000, 0x1fff))
        gb->mbc.mbc1.ramEnable = ((val & 0x0f) == 0x0a);
    else if (IN_RANGE(addr, 0x2000, 0x3fff))
        gb->mbc.mbc1.romBank = (!(val & 0x1f)) ? 1 : val & 0x1f;
    else if (IN_RANGE(addr, 0x4000, 0x5fff))
        gb->mbc.mbc1.ramBank = val & 0x03; 
    else if (IN_RANGE(addr, 0x6000, 0x7fff))
        gb->mbc.mbc1.bankingMode = BIT(val, 0);
    else if (IN_RANGE(addr, 0xa000, 0xbfff)) {
        if (!gb->mbc.mbc1.ramEnable)
            return;
        gb->cart.ram.data[addr - 0xa000 + 0x2000 * gb->mbc.mbc1.ramBank] = val;
    }
}

uint8_t no_mbc_read(struct gb *gb, uint16_t addr)
{
    return gb->cart.rom.data[addr];
}

void no_mbc_write(struct gb *gb, uint16_t addr, uint8_t val)
{

}

uint8_t mbc3_read(struct gb *gb, uint16_t addr)
{
    if (IN_RANGE(addr, 0x0000, 0x3fff))
        return gb->cart.rom.data[addr];
    else if (IN_RANGE(addr, 0x4000, 0x7fff))
        return gb->cart.rom.data[addr - 0x4000 + 0x4000 * gb->mbc.mbc3.romBank];
    else if (IN_RANGE(addr, 0xa000, 0xbfff)) {
        if (gb->mbc.mbc3.ramEnable) {
            // printf("RAM read. Addr - 0x%04x, bank: 0x%02x\n", addr, gb->mbc.mbc3.ramBank);
            if (!gb->mbc.mbc3.ramBank)
                return gb->cart.ram.data[addr - 0xa000];
            else
                return gb->cart.ram.data[addr - 0xa000 + 0x2000 * gb->mbc.mbc3.ramBank];
        }
    }
    return 0xff;
}

void mbc3_write(struct gb *gb, uint16_t addr, uint8_t val)
{
    if (IN_RANGE(addr, 0x0000, 0x1fff)) {
        gb->mbc.mbc3.ramEnable = ((val & 0x0f) == 0x0a);
    } else if (IN_RANGE(addr, 0x2000, 0x3fff)) {
        gb->mbc.mbc3.romBank = (!(val & 0x7f)) ? 1 : val & 0x7f;
    } else if (IN_RANGE(addr, 0x4000, 0x5fff)) {
        gb->mbc.mbc3.ramBank = val;
    } else if (IN_RANGE(addr, 0xa000, 0xbfff)) {
        if (gb->mbc.mbc3.ramEnable) {
			// printf("RAM write. Addr - 0x%04x, val - 0x%02x, bank - %d\n", addr, val, gb->mbc.mbc3.ramBank);
            if (!gb->mbc.mbc3.ramBank)
                gb->cart.ram.data[addr - 0xa000] = val;
            else
                gb->cart.ram.data[addr - 0xa000 + 0x2000 * gb->mbc.mbc3.ramBank] = val;
        }
    }
}

uint8_t (*read_func[])(struct gb *gb, uint16_t addr) = {
    [NO_MBC] = no_mbc_read,
    [MBC1] = mbc1_read,
    [MBC1_RAM] = mbc1_read,
    [MBC1_RAM_BATTERY] = mbc1_read,
    [MBC3_RAM_BATTERY] = mbc3_read,
};

void (*write_func[])(struct gb *gb, uint16_t addr, uint8_t val) = {
    [NO_MBC] = no_mbc_write,
    [MBC1] = mbc1_write,
    [MBC1_RAM] = mbc1_write,
    [MBC1_RAM_BATTERY] = mbc1_write,
    [MBC3_RAM_BATTERY] = mbc3_write,
};

void mbc_init(struct gb *gb)
{
    uint8_t romType = gb->cart.rom.type;

    switch (romType) {
    case MBC1_RAM_BATTERY:
        gb->mbc.mbc1.hasBattery = true;
        break;
    case MBC3_RAM_BATTERY:
    case MBC3_TIMER_BATTERY:
    case MBC3_TIMER_RAM_BATTERY:
        gb->mbc.mbc3.hasBattery = true;
        break;
    default:
        break;
    }
}

void cartridge_get_infos(struct gb *gb)
{
    for (uint16_t i = 0x0134, j = 0; i <= 0x0143; i++) {
        if (gb->cart.rom.data[i] != 0x00)
            gb->cart.rom.name[j++] = gb->cart.rom.data[i];
        else {
            gb->cart.rom.name[j] = '\0';
            break;
        }
    }
    gb->cart.rom.type = gb->cart.rom.data[0x0147];
    gb->cart.rom.size = 32 * KiB * (1 << gb->cart.rom.data[0x0148]);
    gb->cart.ram.size = SRAMSizeInNumber[gb->cart.rom.data[0x0149]];
    gb->cart.rom.bankNumber = gb->cart.rom.size / (16 * KiB);
}    

void cartridge_print_info(struct gb *gb)
{
    printf("name: %s\n", gb->cart.rom.name);
    printf("mbc type: %s\n", ROMType[gb->cart.rom.type]);
    printf("ROM size: %s\n", ROMSize[gb->cart.rom.data[0x0148]]);
    printf("RAM size: %s\n", SRAMSize[gb->cart.rom.data[0x0149]]);
    printf("bank size: %d\n", gb->cart.rom.bankNumber);
}

void cartridge_load(struct gb *gb, uint8_t *rom)
{
    gb->cart.rom.data = rom;
    cartridge_get_infos(gb);
    cartridge_print_info(gb);
    mbc_init(gb);
}

void load_state_after_booting(struct gb *gb)
{
    struct cpu *cpu = &gb->cpu;
    struct ppu *ppu = &gb->ppu;
    struct interrupt *interrupt = &gb->interrupt;
    struct timer *timer = &gb->timer;
    struct dma *dma = &gb->dma;
    struct joypad *joypad = &gb->joypad;
    struct mbc *mbc = &gb->mbc;
    struct serial *serial = &gb->serial;

    gb->mode = NORMAL;

    // cpu
    cpu->pc = 0x100;
    cpu->af.a = 0x01;
    cpu->af.f = 0x80;
    cpu->bc.val = 0x0013;
    cpu->de.val = 0x00d8;
    cpu->hl.val = 0x014d;
    cpu->sp = 0xfffe; 

    // interrupt
    interrupt->flag = 0xe1;
    interrupt->ie = 0x00;

    // timer
    timer->div = 0xab;
    timer->tima = 0x00;
    timer->tma = 0x00;
    timer->tac.val = 0xf8;

    // serial
    serial->sb = 0x00;
    serial->sc = 0x7e;

    // ppu
    ppu->lcdc.val = 0x91;
    uint8_t val = ppu->lcdc.val;
    ppu->lcdc.ppuEnable = BIT(val, 7);
    ppu->lcdc.winTileMap = 0x9800 | (BIT(val, 6) << 10);
    ppu->lcdc.winEnable = BIT(val, 5);
    ppu->lcdc.bgWinTiles = 0x8800 - (0x800 * BIT(val, 4));
    ppu->lcdc.bgTileMap = 0x9800 | (BIT(val, 3) << 10);
    ppu->lcdc.objSize = 0x08 << BIT(val, 2);
    ppu->lcdc.objEnable = BIT(val, 1);
    ppu->lcdc.bgWinEnable = BIT(val, 0);
    ppu->stat.val = 0x85;
    ppu->statIntrLine = 0;
    ppu->scy = 0x00;
    ppu->scx = 0x00;
    ppu->ly = 0x00;
    ppu->lyc = 0x00;
    ppu->pal[BGP] = 0xfc;
    ppu->wy = 0x00;
    ppu->wx = 0x00;
    ppu->ticks = 0;
    ppu->mode = OAM_SCAN;
    ppu->frameReady = false;
    ppu->scanLineReady = false;
    ppu->oamEntryCounter = 0;
    ppu->spriteCounter = 0;
    ppu->statIntrLine = false;
    ppu->windowInFrame = false;
    ppu->windowLineCounter = 0;
    ppu->drawWindowThisLine = false;

    // dma
    dma->mode = OFF;
    dma->reg = 0xff;

    // joypad
    memset(joypad->button, 1, 8);
    joypad->joyp.val = 0xcf;

    // mbc1
    mbc->mbc1.ramEnable = 0;
    mbc->mbc1.bankingMode = 0;
    mbc->mbc1.romBank = 0;
    mbc->mbc1.ramBank = 0;
    mbc->mbc1.hasBattery = false;

    // mbc3
    mbc->mbc3.ramEnable = false;
    mbc->mbc3.romBank = 0;
    mbc->mbc3.ramBank = 0;



    // // TODO: update each channel after booting status 
    // //       when implementing them. 

    // apu->tick = 0;
    // apu->frame_sequencer = 0;

    // // apu square1
    // apu->sqr1.name = SQUARE1;
    // apu->sqr1.is_active = true;
    // apu->sqr1.regs.nrx0 = 0x80;
    // apu->sqr1.regs.nrx1 = 0xbf;
    // apu->sqr1.regs.nrx2 = 0xf3;
    // apu->sqr1.regs.nrx3 = 0xff;
    // apu->sqr1.regs.nrx4 = 0xbf;
    // apu->sqr1.is_dac_on = is_dac_on(&gb->apu.sqr1);
    // apu->sqr1.length.counter = get_length_load(&gb->apu.sqr1);
    // apu->sqr1.timer = get_frequency(&gb->apu.sqr1);
    // apu->sqr1.left_output = true;
    // apu->sqr1.right_output = true;
    // apu->sqr1.volume_envelope.add_mode = get_envelope_add_mode(&gb->apu.sqr1);
    // apu->sqr1.volume = 15;
    // apu->sqr1.pos = 0;
    // apu->sqr1.volume_envelope.period = get_envelope_period(&gb->apu.sqr1);
    // apu->sqr1.frequency_sweep.period = get_sweep_period(&gb->apu.sqr1);
    // apu->sqr1.frequency_sweep.negate = BIT(gb->apu.sqr1.regs.nrx0, 3);
    // apu->sqr1.frequency_sweep.shift = get_sweep_shift(&gb->apu.sqr1);
    // apu->sqr1.frequency_sweep.is_active = true;
    // apu->sqr1.frequency_sweep.timer = gb->apu.sqr1.frequency_sweep.period;
    // apu->sqr1.frequency_sweep.shadow_frequency = get_frequency(&gb->apu.sqr1);

    // // apu square2
    // apu->sqr2.name = SQUARE2;
    // apu->sqr2.is_active = false;
    // apu->sqr2.regs.nrx1 = 0x3f;
    // apu->sqr2.regs.nrx2 = 0x00;
    // apu->sqr2.regs.nrx3 = 0xff;
    // apu->sqr2.regs.nrx4 = 0xbf;
    // apu->sqr2.is_dac_on = is_dac_on(&gb->apu.sqr2);
    // apu->sqr2.length.counter = 0;
    // apu->sqr2.timer = 0;
    // apu->sqr2.left_output = true;
    // apu->sqr2.right_output = true;
    // apu->sqr2.volume_envelope.add_mode = get_envelope_add_mode(&gb->apu.sqr2);
    // apu->sqr2.volume = 0;
    // apu->sqr2.pos = 0;
    // apu->sqr2.volume_envelope.period = get_envelope_period(&gb->apu.sqr2);

    // // apu wave
    // apu->wave.name = WAVE;
    // apu->wave.is_active = false;
    // apu->wave.regs.nrx0 = 0x7f;
    // apu->wave.regs.nrx1 = 0xff;
    // apu->wave.regs.nrx2 = 0x9f;
    // apu->wave.regs.nrx3 = 0xff;
    // apu->wave.regs.nrx4 = 0xbf;
    // apu->wave.is_dac_on = BIT(gb->apu.wave.regs.nrx0, 7);
    // apu->wave.left_output = true;
    // apu->wave.right_output = false;
    // apu->wave.pos = 0;
    // apu->wave.is_dac_on = is_dac_on(&apu->wave);

    // // apu noise
    // apu->noise.name = NOISE;
    // apu->noise.is_active = false;
    // apu->noise.regs.nrx1 = 0xff;
    // apu->noise.regs.nrx2 = 0x00;
    // apu->noise.regs.nrx3 = 0x00;
    // apu->noise.regs.nrx4 = 0xbf;
    // apu->noise.left_output = true;
    // apu->noise.right_output = false;
    // apu->noise.is_dac_on = is_dac_on(&gb->apu.noise);
    // apu->noise.lfsr.clock_shift = get_noise_clock_shift(&gb->apu.noise);
    // apu->noise.lfsr.divisor = get_noise_divisor(&gb->apu.noise);
    // apu->noise.lfsr.width_mode = get_noise_width_mode(&gb->apu.noise);
    // apu->noise.timer = apu->noise.lfsr.divisor << apu->noise.lfsr.clock_shift;
    // apu->noise.volume = get_envelope_volume(&gb->apu.noise);
    // apu->noise.volume_envelope.add_mode = get_envelope_add_mode(&gb->apu.noise);
    // apu->noise.volume_envelope.period = get_envelope_period(&gb->apu.noise);
    // apu->noise.lfsr.reg = 0x7fff;

    // // apu global control
    // apu->ctrl.name = CTRL;
    // apu->ctrl.regs.nrx0 = 0x77;
    // apu->ctrl.regs.nrx1 = 0xf3;
    // apu->ctrl.regs.nrx2 = 0xf1;

    // // sound panning
    // apu->noise.left_output = true;
    // apu->wave.left_output = true;
    // apu->sqr2.left_output = true;
    // apu->sqr1.left_output = true;
    // apu->noise.right_output = false;
    // apu->wave.right_output = false;
    // apu->sqr2.right_output = true;
    // apu->sqr1.right_output = true;

    // apu->sample_buffer.ptr = 0;
    // memset(apu->sample_buffer.buf, 0, BUFFER_SIZE * sizeof(int16_t));
    // apu->sample_buffer.is_full = false;
}

/**********************************************************************************************/
/************************************* bus related parts **************************************/
/**********************************************************************************************/



uint8_t bus_read(struct gb *gb, uint16_t addr)
{
    uint8_t ret = 0xff, romBank;

    switch (GET_MEM_REGION(addr)) {
    case ROM:
        return read_func[gb->cart.rom.type](gb, addr);
    case vRAM:
        return gb->vRAM[addr - 0x8000];
    case externalRAM:
        if (gb->cart.ram.size > 0)
            return read_func[gb->cart.rom.type](gb, addr);
        break;
    case WRAM:
        return gb->workRAM[addr - 0xc000];
    case ECHO_RAM:
        return gb->workRAM[(addr & 0xddff) - 0xc000];
    case OAM:
        return gb->oam[addr - 0xfe00];
    case UNUSED:
        return gb->unused[addr - 0xfea0];
    case IO:
        switch (WHICH_IO_REGION(addr)) {
        case JOYPAD:
            if (gb->joypad.joyp.selectButton && gb->joypad.joyp.selectDPad)
                return 0xff;
            else if (!gb->joypad.joyp.selectDPad)
                return (gb->joypad.joyp.val & 0xf0) | (gb->joypad.button[JOYPAD_RIGHT] << 0) | (gb->joypad.button[JOYPAD_LEFT] << 1) 
                        | (gb->joypad.button[JOYPAD_UP] << 2) | (gb->joypad.button[JOYPAD_DOWN] << 3);
            else if (!gb->joypad.joyp.selectButton)
                return (gb->joypad.joyp.val & 0xf0) | (gb->joypad.button[JOYPAD_A] << 0) | (gb->joypad.button[JOYPAD_B] << 1) |
                         (gb->joypad.button[JOYPAD_SELECT] << 2) | (gb->joypad.button[JOYPAD_START] << 3);
            break;
        case TIMER:
            switch (addr) {
            case TIMER_REG_DIV:
                return (gb->timer.div >> 6) & 0x00ff;
            case TIMER_REG_TAC:
                return gb->timer.tac.val;
            case TIMER_REG_TIMA:
                return gb->timer.tima;
            case TIMER_REG_TMA:
                return gb->timer.tma;
            default:
                break;
            }
        case INTERRUPT:
            switch (addr) {
            case INTERRUPT_REG_IE:
                return gb->interrupt.ie;
            case INTERRUPT_REG_IF:
                return gb->interrupt.flag;
            default:
                break;
            }
        case PPU:
            switch (addr) {
            case PPU_REG_LCDC:
                return gb->ppu.lcdc.val;
            case PPU_REG_STAT:
                return gb->ppu.stat.val;
            case PPU_REG_SCY:
                return gb->ppu.scy;
            case PPU_REG_SCX:
                return gb->ppu.scx;
            case PPU_REG_LY:
                return gb->ppu.ly;
            case PPU_REG_LYC:
                return gb->ppu.lyc;
            case PPU_REG_BGP:
                return gb->ppu.pal[BGP];
            case PPU_REG_OBP0:
                return gb->ppu.pal[OBP0];
            case PPU_REG_OBP1:
                return gb->ppu.pal[OBP1];
            case PPU_REG_WY:
                return gb->ppu.wy;
            case PPU_REG_WX:
                return gb->ppu.wx;
            case DMA_REG_OAM:
                return gb->dma.reg;
            default:
                break;
            }
        default:
            break;
        }
        break;
    case HRAM:
        return gb->highRAM[addr - 0xff80];
    default:
        break;
    }
    return 0xff;
}

void bus_write(struct gb *gb, uint16_t addr, uint8_t val)
{
    switch (GET_MEM_REGION(addr)) {
    case ROM:
        write_func[gb->cart.rom.type](gb, addr, val);
        break;
    case vRAM:
        gb->vRAM[addr - 0x8000] = val;
        break;
    case externalRAM:
        if (gb->cart.ram.size > 0)
            write_func[gb->cart.rom.type](gb, addr, val);
        break;
    case WRAM:
        gb->workRAM[addr - 0xc000] = val;
        break;
    case ECHO_RAM:
        gb->workRAM[(addr & 0xddff) - 0xc000] = val;
        break;
    case OAM:
        gb->oam[addr - 0xfe00] = val;
        break;
    case UNUSED:
        gb->unused[addr - 0xfea0] = val;
        break;
    case IO:
        switch (WHICH_IO_REGION(addr)) {
        case JOYPAD:
            gb->joypad.joyp.val = val | (gb->joypad.joyp.val & 0xcf) | 0xc0;
            break;
        case INTERRUPT:
            switch (addr){
            case INTERRUPT_REG_IE:
                gb->interrupt.ie = val;
                break;
            case INTERRUPT_REG_IF:
                gb->interrupt.flag = val;
                break;
            }
            break;
        case TIMER:
            switch (addr) {
            case TIMER_REG_DIV:
                gb->timer.div = 0;
                break;
            case TIMER_REG_TAC:
                gb->timer.tac.val = val;
                break;
            case TIMER_REG_TIMA:
                gb->timer.tima = val;
                break;
            case TIMER_REG_TMA:
                gb->timer.tma = val;
                break;
            default:
                break;
            }
            break;
        case PPU:
            switch (addr) {
                case PPU_REG_LCDC:
                    gb->ppu.lcdc.val = val;
                    // extract informations from LCDC
                    gb->ppu.lcdc.ppuEnable = BIT(val, 7);
                    gb->ppu.lcdc.winTileMap = 0x9800 | (BIT(val, 6) << 10);
                    gb->ppu.lcdc.winEnable = BIT(val, 5);
                    gb->ppu.lcdc.bgWinTiles = 0x8800 - (0x800 * BIT(val, 4));
                    gb->ppu.lcdc.bgTileMap = 0x9800 | (BIT(val, 3) << 10);
                    gb->ppu.lcdc.objSize = 0x08 << BIT(val, 2);
                    gb->ppu.lcdc.objEnable = BIT(val, 1);
                    gb->ppu.lcdc.bgWinEnable = BIT(val, 0);
                    if (!gb->ppu.lcdc.ppuEnable) {
                        SET_MODE(HBLANK);
                        gb->ppu.ly = 0;
                        gb->ppu.ticks = 0;
                    }
                    break;
                case PPU_REG_STAT:
                    gb->ppu.stat.val = (val & 0xf8) | (gb->ppu.stat.val & 0x87);
                    break;
                case PPU_REG_SCY:
                    gb->ppu.scy = val;
                    break; 
                case PPU_REG_SCX:
                    gb->ppu.scx = val;
                    break; 
                case PPU_REG_LYC:
                    gb->ppu.lyc = val;
                    gb->ppu.stat.lycEqualLy = gb->ppu.lyc == gb->ppu.ly;
                    break; 
                case PPU_REG_BGP:
                    gb->ppu.pal[BGP] = val;
                    break;
                case PPU_REG_OBP0:
                    gb->ppu.pal[OBP0] = val;
                    break;
                case PPU_REG_OBP1:
                    gb->ppu.pal[OBP1] = val;
                    break;
                case PPU_REG_WY:
                    gb->ppu.wy = val;
                    gb->ppu.windowInFrame = gb->ppu.wy == gb->ppu.ly;
                    break;
                case PPU_REG_WX:
                    gb->ppu.wx = val;
                    break;
                case DMA_REG_OAM:
                    gb->dma.reg = val;
                    gb->dma.mode = WAITING;
                    gb->dma.startAddr = TO_U16(0x00, val);
                    gb->dma.tick = 0;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }
        break;
    case HRAM:
        gb->highRAM[addr - 0xff80] = val;
        break;
    default:
        break;
    }
}

void interrupt_request(struct gb *gb, uint8_t intr_src)
{
    gb->interrupt.flag |= intr_src;
}