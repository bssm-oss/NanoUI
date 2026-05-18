#ifndef NANOUI_CONFIG_H
#define NANOUI_CONFIG_H

/**
 * @file config.h
 * @brief Default pin mappings for NanoUI.
 *
 * Override any define before including NanoUI.h, or pass -D flags
 * via platformio.ini build_flags to customise for your board.
 */

// ── SPI ILI9341 defaults ────────────────────────────────────────────────────
#ifndef NANOUI_SPI_TFT_CS
#define NANOUI_SPI_TFT_CS   10
#endif
#ifndef NANOUI_SPI_TFT_DC
#define NANOUI_SPI_TFT_DC    9
#endif
#ifndef NANOUI_SPI_TFT_RST
#define NANOUI_SPI_TFT_RST   8
#endif
#ifndef NANOUI_SPI_TOUCH_CS
#define NANOUI_SPI_TOUCH_CS  7
#endif

// ── 8-bit Parallel ILI9341 defaults (LOLIN D32) ─────────────────────────────
#ifndef NANOUI_PAR_CS
#define NANOUI_PAR_CS       27
#endif
#ifndef NANOUI_PAR_DC
#define NANOUI_PAR_DC        2
#endif
#ifndef NANOUI_PAR_WR
#define NANOUI_PAR_WR        4
#endif
#ifndef NANOUI_PAR_RST
#define NANOUI_PAR_RST      33
#endif

#ifndef NANOUI_PAR_DB0
#define NANOUI_PAR_DB0      12
#endif
#ifndef NANOUI_PAR_DB1
#define NANOUI_PAR_DB1      13
#endif
#ifndef NANOUI_PAR_DB2
#define NANOUI_PAR_DB2      14
#endif
#ifndef NANOUI_PAR_DB3
#define NANOUI_PAR_DB3      15
#endif
#ifndef NANOUI_PAR_DB4
#define NANOUI_PAR_DB4      21
#endif
#ifndef NANOUI_PAR_DB5
#define NANOUI_PAR_DB5      22
#endif
#ifndef NANOUI_PAR_DB6
#define NANOUI_PAR_DB6      25
#endif
#ifndef NANOUI_PAR_DB7
#define NANOUI_PAR_DB7      26
#endif

#ifndef NANOUI_PAR_TOUCH_CS
#define NANOUI_PAR_TOUCH_CS  5
#endif

// ── Touch SPI bus (XPT2046) ─────────────────────────────────────────────────
#ifndef NANOUI_TOUCH_SPI_SCK
#define NANOUI_TOUCH_SPI_SCK   18
#endif
#ifndef NANOUI_TOUCH_SPI_MISO
#define NANOUI_TOUCH_SPI_MISO  19
#endif
#ifndef NANOUI_TOUCH_SPI_MOSI
#define NANOUI_TOUCH_SPI_MOSI  23
#endif

#endif // NANOUI_CONFIG_H
