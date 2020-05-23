// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020- Aodzip
 * All rights reserved.
 * Author: Aodzip <aodzip@gmail.com>
 */
#ifndef __SUN6I_MIPI_REG_H__
#define __SUN6I_MIPI_REG_H__

#include <linux/kernel.h>

#define MIPI_CSI2_OFFSET 0x1000

#define MIPI_CSI2_CTL_REG (MIPI_CSI2_OFFSET + 0x0000)
#define MIPI_CSI2_CTL_RST BIT(31)
#define MIPI_CSI2_CTL_EN BIT(0)
/* MIPI_CSI2_CTL_REG */

#define MIPI_CSI2_CFG_REG (MIPI_CSI2_OFFSET + 0x0004)
#define MIPI_CSI2_CFG_CH_MOD_SHIFT 8
#define MIPI_CSI2_CFG_CH_MOD GENMASK(9, MIPI_CSI2_CFG_CH_MOD_SHIFT)
#define MIPI_CSI2_CFG_DL_CFG_SHIFT 0
#define MIPI_CSI2_CFG_DL_CFG GENMASK(1, MIPI_CSI2_CFG_DL_CFG_SHIFT)
/* MIPI_CSI2_CFG_REG */

#define MIPI_CSI2_VCDT_RX_REG (MIPI_CSI2_OFFSET + 0x0008)
#define MIPI_CSI2_VCDT_RX_REG_VCDT(vc, dt) (((vc & 0b11) << 6) | (dt & 0b111111))
#define MIPI_CSI2_VCDT_RX_REG_CH_DATA(data, ch) (data << (ch * 8))
#define MIPI_CSI2_VCDT_RX_REG_CH_MASK(ch) (MIPI_CSI2_VCDT_RX_REG_CH_DATA(0xFF, ch))
#define MIPI_CSI2_VCDT_RX_REG_CH_CONF(ch, dt) (MIPI_CSI2_VCDT_RX_REG_CH_DATA(MIPI_CSI2_VCDT_RX_REG_VCDT(ch, dt), ch))
/* MIPI_CSI2_VCDT_RX_REG */

#define MIPI_CSI2_CH_CFG_REG (MIPI_CSI2_OFFSET + 0x0040)
#define MIPI_CSI2_CH_CFG_SRC_SEL BIT(3)

/* MIPI_CSI2_CH_CFG_REG */

#endif /* __SUN6I_MIPI_REG_H__ */