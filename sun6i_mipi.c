// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020- Aodzip
 * All rights reserved.
 * Author: Aodzip <aodzip@gmail.com>
 */
#include "sun6i_mipi.h"
#include "sun6i_mipi_reg.h"
#include <linux/regmap.h>
#include <linux/delay.h>
#include <uapi/linux/media-bus-format.h>
#include "sun6i_dphy.h"

enum pkt_fmt {
	MIPI_FS = 0X00, /* short packet */
	MIPI_FE = 0X01,
	MIPI_LS = 0X02,
	MIPI_LE = 0X03,
	MIPI_SDAT0 = 0X08,
	MIPI_SDAT1 = 0X09,
	MIPI_SDAT2 = 0X0A,
	MIPI_SDAT3 = 0X0B,
	MIPI_SDAT4 = 0X0C,
	MIPI_SDAT5 = 0X0D,
	MIPI_SDAT6 = 0X0E,
	MIPI_SDAT7 = 0X0F,
	/* NULL          = 0X10, //long packet */
	MIPI_BLK = 0X11,
	MIPI_EMBD = 0X12,
	MIPI_YUV420 = 0X18,
	MIPI_YUV420_10 = 0X19,
	MIPI_YUV420_CSP = 0X1C,
	MIPI_YUV420_CSP_10 = 0X1D,
	MIPI_YUV422 = 0X1E,
	MIPI_YUV422_10 = 0X1F,
	MIPI_RGB565 = 0X22,
	MIPI_RGB888 = 0X24,
	MIPI_RAW8 = 0X2A,
	MIPI_RAW10 = 0X2B,
	MIPI_RAW12 = 0X2C,
	MIPI_USR_DAT0 = 0X30,
	MIPI_USR_DAT1 = 0X31,
	MIPI_USR_DAT2 = 0X32,
	MIPI_USR_DAT3 = 0X33,
	MIPI_USR_DAT4 = 0X34,
	MIPI_USR_DAT5 = 0X35,
	MIPI_USR_DAT6 = 0X36,
	MIPI_USR_DAT7 = 0X37,
};

#define IS_FLAG(x, y) (((x) & (y)) == y)

static inline struct sun6i_csi_dev *sun6i_csi_to_dev(struct sun6i_csi *csi)
{
	return container_of(csi, struct sun6i_csi_dev, csi);
}

static enum pkt_fmt get_pkt_fmt(u16 bus_pix_code)
{
	switch (bus_pix_code) {
	case MEDIA_BUS_FMT_RGB565_1X16:
		return MIPI_RGB565;
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_UYVY8_1X16:
		return MIPI_YUV422;
	case MEDIA_BUS_FMT_UYVY10_2X10:
		return MIPI_YUV422_10;
	case MEDIA_BUS_FMT_RGB888_1X24:
		return MIPI_RGB888;
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		return MIPI_RAW8;
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		return MIPI_RAW10;
	case MEDIA_BUS_FMT_SBGGR12_1X12:
	case MEDIA_BUS_FMT_SGBRG12_1X12:
	case MEDIA_BUS_FMT_SGRBG12_1X12:
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		return MIPI_RAW12;
	default:
		return MIPI_RAW8;
	}
}

void sun6i_mipi_set_power(struct sun6i_csi *csi, bool enable)
{
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);

	if (enable) {
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CTL_REG,
				  MIPI_CSI2_CTL_RST, MIPI_CSI2_CTL_RST);
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CTL_REG,
				  MIPI_CSI2_CTL_EN, MIPI_CSI2_CTL_EN);
		usleep_range(10000, 12000);
		sun6i_dphy_enable(sdev);
	} else {
		sun6i_dphy_disable(sdev);
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CTL_REG,
				  MIPI_CSI2_CTL_EN, 0);
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CTL_REG,
				  MIPI_CSI2_CTL_RST, 0);
	}
}

int sun6i_mipi_set_param(struct sun6i_csi *csi, struct sun6i_mipi_param *param)
{
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);
	struct sun6i_dphy_param dphy_param = {0};
	int lane_num, total_rx_ch;
	int i;

	if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_4_LANE))
		lane_num = 4;
	else if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_3_LANE))
		lane_num = 3;
	else if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_2_LANE))
		lane_num = 2;
	else
		lane_num = 1;

	total_rx_ch = 0;
	if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_CHANNEL_0))
		total_rx_ch++;

	if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_CHANNEL_1))
		total_rx_ch++;

	if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_CHANNEL_2))
		total_rx_ch++;

	if (IS_FLAG(param->cfg.flags, V4L2_MBUS_CSI2_CHANNEL_3))
		total_rx_ch++;

	if (!total_rx_ch) {
		dev_dbg(sdev->dev,
			 "No receive channel assigned, using channel 0.\n");
		total_rx_ch++;
	}

	regmap_write_bits(sdev->regmap, MIPI_CSI2_CFG_REG, MIPI_CSI2_CFG_DL_CFG,
			  (lane_num - 1) << MIPI_CSI2_CFG_DL_CFG_SHIFT);
	regmap_write_bits(sdev->regmap, MIPI_CSI2_CFG_REG, MIPI_CSI2_CFG_CH_MOD,
			  (total_rx_ch - 1) << MIPI_CSI2_CFG_CH_MOD_SHIFT);

	for (i = 0; i < total_rx_ch; i++) {
		regmap_write_bits(sdev->regmap, 
			MIPI_CSI2_VCDT_RX_REG,
			MIPI_CSI2_VCDT_RX_REG_CH(0xFF, i),
			MIPI_CSI2_VCDT_RX_REG_CH(
				MIPI_CSI2_VCDT_RX_REG_VCDT(i, get_pkt_fmt(param->fmt.format.code)),
				i
			)
		);
	}

	if (param->fmt.format.field == V4L2_FIELD_NONE ||
	    param->fmt.format.field == V4L2_FIELD_ANY) {
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CH_CFG_REG,
				  MIPI_CSI2_CH_CFG_SRC_SEL, 0);
	} else {
		regmap_write_bits(sdev->regmap, MIPI_CSI2_CH_CFG_REG,
				  MIPI_CSI2_CH_CFG_SRC_SEL,
				  MIPI_CSI2_CH_CFG_SRC_SEL);
	}

	dphy_param.lane_num = lane_num;
	dphy_param.bps = param->bps;
	dphy_param.auto_bps = 1;
	sun6i_dphy_set_param(sdev, &dphy_param);

	return 0;
}
