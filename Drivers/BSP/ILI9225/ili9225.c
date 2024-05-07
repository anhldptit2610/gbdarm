#include "ili9225.h"

struct commandAndData {
    uint16_t command;
    uint16_t data;
};

#define ARRAYSIZE(array)    (sizeof(array)/sizeof(array[0]))

void ili9225_set_cs(state csState)
{
    HAL_GPIO_WritePin(ILI9225_CS_GPIO_Port, ILI9225_CS_Pin, csState);
}

void ili9225_set_rst(state rstState)
{
    HAL_GPIO_WritePin(ILI9225_RST_GPIO_Port, ILI9225_RST_Pin, rstState);
}

void ili9225_set_dc(state dcState)
{
    HAL_GPIO_WritePin(ILI9225_DC_GPIO_Port, ILI9225_DC_Pin, dcState);
}

void ili9225_write_16(uint16_t value)
{
    HAL_SPI_Transmit(&hspi4, (uint8_t *)&value, 1, 100);
}

void ili9225_write_reg(uint16_t reg)
{
    ili9225_set_cs(STATE_ENABLE);
    ili9225_set_dc(COMMAND);
    ili9225_write_16(reg);
    ili9225_set_cs(STATE_DISABLE);
}

void ili9225_write_cmd(uint16_t cmd, uint16_t value)
{
    ili9225_set_cs(STATE_ENABLE);
    ili9225_set_dc(COMMAND);
    ili9225_write_16(cmd);
    ili9225_set_dc(DATA);
    ili9225_write_16(value);
    ili9225_set_cs(STATE_DISABLE);
}

void ili9225_init(void)
{
    /* ILI9225 init */
    ili9225_set_rst(STATE_DISABLE);
    ili9225_set_cs(STATE_DISABLE);
    ili9225_set_dc(COMMAND);
    HAL_Delay(1);

    ili9225_set_rst(STATE_ENABLE);
    HAL_Delay(50);
    ili9225_set_rst(STATE_DISABLE);
    HAL_Delay(100);

    {
        struct commandAndData commands[] = {
            {ILI9225_POWER_CTRL1, 0x0000},
            {ILI9225_POWER_CTRL2, 0x0000},
            {ILI9225_POWER_CTRL3, 0x0000},
            {ILI9225_POWER_CTRL4, 0x0000},
            {ILI9225_POWER_CTRL5, 0x0000},
        };
        for (int i = 0; i < 5; i++)
            ili9225_write_cmd(commands[i].command, commands[i].data);
        HAL_Delay(40);
    }

    {
        struct commandAndData commands[] = {
            {ILI9225_POWER_CTRL2, 0x0018},
            {ILI9225_POWER_CTRL3, 0x6121},
            {ILI9225_POWER_CTRL4, 0x006f},
            {ILI9225_POWER_CTRL5, 0x495f},
            {ILI9225_POWER_CTRL1, 0x0800},
        };
        for (int i = 0; i < 5; i++)
            ili9225_write_cmd(commands[i].command, commands[i].data);
        HAL_Delay(10);
    }

    ili9225_write_cmd(ILI9225_POWER_CTRL2, 0x103b);
    HAL_Delay(50);

    {
        struct commandAndData commands[] = {
			{ILI9225_DRIVER_OUTPUT_CTRL,	0x011C},
			/* Set LCD inversion to disabled. */
			{ILI9225_LCD_AC_DRIVING_CTRL, 0x0100},
			/* Increment vertical and horizontal address.
			 * Use vertical image. */
			{ILI9225_ENTRY_MODE, 0x1028},
			/* Turn off all display outputs. */
			{ILI9225_DISP_CTRL1,	0x0000},
			/* Set porches to 8 lines. */
			{ILI9225_BLANK_PERIOD_CTRL1, 0x0808},
			/* Use 1-clock delay to gate output and edge. */
			{ILI9225_FRAME_CYCLE_CTRL, 0x1100},
			/* Ignore RGB interface settings. */
			{ILI9225_INTERFACE_CTRL, 0x0000},
			/* Set oscillation frequency to 266.6 kHz. */
			{ILI9225_OSC_CTRL, 0x0f01},
			/* Set VCI recycling to 2 clocks. */
			{ILI9225_VCI_RECYCLING, 0x0020},
			/* Initialise RAM Address to 0x0 px. */
			{ILI9225_RAM_ADDR_SET1, 0x0000},
			{ILI9225_RAM_ADDR_SET2, 0x0000},

			/* Set scanning position to full screen. */
			{ILI9225_GATE_SCAN_CTRL,	0x0000},
			/* Set end scan position to 219 + 1 px (0xDB). */
			{ILI9225_VERTICAL_SCROLL_CTRL1,	0x00DB},
			/* Set start scan position to 0 px. */
			{ILI9225_VERTICAL_SCROLL_CTRL2,	0x0000},
			/* Set scroll step to 0 px (no scrolling). */
			{ILI9225_VERTICAL_SCROLL_CTRL3,	0x0000},
			/* Set partial screen driving end to 219 + 1 px
			 * (0xDB). */
			{ILI9225_PARTIAL_DRIVING_POS1, 0x00DB},
			/* Set partial screen driving start to 0 px. */
			{ILI9225_PARTIAL_DRIVING_POS2, 0x0000},
			/* Set window to 176 x 220 px (full screen). */
			// {ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF},
			// {ILI9225_HORIZONTAL_WINDOW_ADDR2,	0x0000},
			// {ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB},
			// {ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000},
			{ILI9225_HORIZONTAL_WINDOW_ADDR1, 153},
			{ILI9225_HORIZONTAL_WINDOW_ADDR2,	9},
			{ILI9225_VERTICAL_WINDOW_ADDR1, 159},
			{ILI9225_VERTICAL_WINDOW_ADDR2, 0},
			/* Gamma curve data. */
			{ILI9225_GAMMA_CTRL1, 0x0000},
			{ILI9225_GAMMA_CTRL2, 0x0808},
			{ILI9225_GAMMA_CTRL3, 0x080A},
			{ILI9225_GAMMA_CTRL4, 0x000A},
			{ILI9225_GAMMA_CTRL5, 0x0A08},
			{ILI9225_GAMMA_CTRL6, 0x0808},
			{ILI9225_GAMMA_CTRL7, 0x0000},
			{ILI9225_GAMMA_CTRL8, 0x0A00},
			{ILI9225_GAMMA_CTRL9, 0x0710},
			{ILI9225_GAMMA_CTRL10, 0x0710},

			/* Enable full colour display. */
			{ILI9225_DISP_CTRL1, 0x0012}
        };
        for (int i = 0; i < ARRAYSIZE(commands); i++)
            ili9225_write_cmd(commands[i].command, commands[i].data);
        HAL_Delay(50);
    }

    ili9225_write_cmd(ILI9225_DISP_CTRL1, 0x1017);
    HAL_Delay(50);
}

void ili9225_set_window_area(uint16_t verticalStart, uint16_t verticalEnd, uint16_t horizontalStart, uint16_t horizontalEnd)
{
    ili9225_write_cmd(ILI9225_HORIZONTAL_WINDOW_ADDR2, horizontalStart);
    ili9225_write_cmd(ILI9225_HORIZONTAL_WINDOW_ADDR1, horizontalEnd);
    ili9225_write_cmd(ILI9225_VERTICAL_WINDOW_ADDR2, verticalStart);
    ili9225_write_cmd(ILI9225_VERTICAL_WINDOW_ADDR1, verticalEnd);
}

void ili9225_set_gram_ptr(uint16_t horizontal, uint16_t vertical)
{
    ili9225_write_cmd(ILI9225_RAM_ADDR_SET1, horizontal);
    ili9225_write_cmd(ILI9225_RAM_ADDR_SET2, vertical);
}

void ili9225_draw_bitmap(uint16_t *bitMap, uint16_t width, uint16_t height, transferMethod xferMethod)
{
    ili9225_write_reg(ILI9225_GRAM_DATA_REG);
    ili9225_set_cs(STATE_ENABLE);
    ili9225_set_dc(DATA);
    if (xferMethod == PLAINSPI) {
        HAL_SPI_Transmit(&hspi4, (uint8_t *)bitMap, width * height, 100);
    } else if (xferMethod == DMA) {
        HAL_SPI_Transmit_DMA(&hspi4, (uint8_t *)bitMap, width * height);
    }
    ili9225_set_cs(STATE_DISABLE);
}