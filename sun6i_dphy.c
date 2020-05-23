// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020- Aodzip
 * All rights reserved.
 * Author: Aodzip <aodzip@gmail.com>
 */
#include "sun6i_dphy.h"
#include "sun6i_dphy_reg.h"

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/regmap.h>

#define DPHY_CLK (150 * 1000 * 1000)

void sun6i_dphy_set_data_lane(struct regmap *regmap, unsigned char lane_num)
{
	regmap_write_bits(regmap, DPHY_CTL_REG, DPHY_CTL_REG_LANE_NUM,
			  (lane_num - 1) << DPHY_CTL_REG_LANE_NUM_SHIFT);
}

void sun6i_dphy_rx_enable(struct regmap *regmap, unsigned char lane_num)
{
	regmap_write_bits(regmap, DPHY_RX_CTL_REG, DPHY_RX_CTL_REG_RX_CLK_FORCE,
			  1 << DPHY_RX_CTL_REG_RX_CLK_FORCE_SHIFT);
	regmap_write_bits(regmap, DPHY_RX_CTL_REG,
			  DPHY_RX_CTL_REG_RX_DATA_FORCE,
			  GENMASK(lane_num - 1, 0) << DPHY_RX_CTL_REG_RX_DATA_FORCE_SHIFT);
}

void sun6i_dphy_rx_disable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_CTL_REG, DPHY_RX_CTL_REG_RX_CLK_FORCE,
			  0 << DPHY_RX_CTL_REG_RX_CLK_FORCE_SHIFT);
	regmap_write_bits(regmap, DPHY_RX_CTL_REG,
			  DPHY_RX_CTL_REG_RX_DATA_FORCE, 0);
}

void sun6i_dphy_rx_dbc_enable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_CTL_REG, DPHY_RX_CTL_REG_DBC_EN,
			  1 << DPHY_RX_CTL_REG_DBC_EN_SHIFT);
}

void sun6i_dphy_rx_hs_clk_miss_cnt_disable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_HSRX_CLK_MISS_EN,
			  0 << DPHY_RX_TIME0_REG_HSRX_CLK_MISS_EN_SHIFT);
}

void sun6i_dphy_rx_hs_sync_cnt_disable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_HSRX_SYNC_ERR_TO_EN,
			  0 << DPHY_RX_TIME0_REG_HSRX_SYNC_ERR_TO_EN_SHIFT);
}

void sun6i_dphy_rx_lp_to_cnt_disable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_LPRX_TO_EN,
			  0 << DPHY_RX_TIME0_REG_LPRX_TO_EN_SHIFT);
}

void sun6i_dphy_rx_freq_cnt_enable(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_FREQ_CNT_EN,
			  1 << DPHY_RX_TIME0_REG_FREQ_CNT_EN_SHIFT);
}

void sun6i_dphy_rx_set_hs_clk_miss(struct regmap *regmap, unsigned char cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_HSRX_CLK_MISS,
			  cnt << DPHY_RX_TIME0_REG_HSRX_CLK_MISS_SHIFT);
}

void sun6i_dphy_rx_set_hs_sync_to(struct regmap *regmap, unsigned char cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG,
			  DPHY_RX_TIME0_REG_HSRX_SYNC_ERR_TO,
			  cnt << DPHY_RX_TIME0_REG_HSRX_SYNC_ERR_TO_SHIFT);
}

void sun6i_dphy_rx_set_lp_to(struct regmap *regmap, unsigned char cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME0_REG, DPHY_RX_TIME0_REG_LPRX_TO,
			  cnt << DPHY_RX_TIME0_REG_LPRX_TO_SHIFT);
}

void sun6i_dphy_rx_set_rx_dly(struct regmap *regmap, unsigned short cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME1_REG, DPHY_RX_TIME1_REG_RX_DLY,
			  cnt << DPHY_RX_TIME1_REG_RX_DLY_SHIFT);
}

void sun6i_dphy_rx_set_lprst_dly(struct regmap *regmap, unsigned char cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME3_REG,
			  DPHY_RX_TIME3_REG_LPRST_DLY,
			  cnt << DPHY_RX_TIME3_REG_LPRST_DLY_SHIFT);
}

void sun6i_dphy_rx_set_entm_to_enrx_dly(struct regmap *regmap,
					unsigned char cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME2_REG,
			  DPHY_RX_TIME2_REG_HSRX_ANA0_SET,
			  cnt << DPHY_RX_TIME2_REG_HSRX_ANA0_SET_SHIFT);
}

void sun6i_dphy_rx_set_lp_ulps_wp(struct regmap *regmap, unsigned int cnt)
{
	regmap_write_bits(regmap, DPHY_RX_TIME1_REG,
			  DPHY_RX_TIME1_REG_LPRX_ULPS_WP,
			  cnt << DPHY_RX_TIME1_REG_LPRX_ULPS_WP_SHIFT);
}

unsigned short sun6i_dphy_rx_get_freq_cnt(struct regmap *regmap)
{
	unsigned int status;
	regmap_read(regmap, DPHY_RX_TIME3_REG, &status);
	return (status & DPHY_RX_TIME3_REG_FREQ_CNT) >>
	       DPHY_RX_TIME3_REG_FREQ_CNT_SHIFT;
}

void sun6i_dphy_ana_init(struct regmap *regmap)
{
	/* init setting */
	regmap_write_bits(regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_PWS,
			  1 << DPHY_ANA0_REG_PWS_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_SFB,
			  2 << DPHY_ANA0_REG_SFB_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_SLV,
			  7 << DPHY_ANA0_REG_SLV_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA1_REG, DPHY_ANA1_REG_SVTT,
			  4 << DPHY_ANA1_REG_SVTT_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_DMPC,
			  0 << DPHY_ANA0_REG_DMPC_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_DMP,
			  0 << DPHY_ANA0_REG_DMP_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA4_REG, DPHY_ANA4_REG_DMPLVC,
			  1 << DPHY_ANA4_REG_DMPLVC_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA4_REG, DPHY_ANA4_REG_DMPLVD,
			  1 << DPHY_ANA4_REG_DMPLVD_SHIFT);

	/* ctl init */
	regmap_write_bits(regmap, DPHY_ANA2_REG, DPHY_ANA2_REG_ENIB,
			  1 << DPHY_ANA2_REG_ENIB_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOR,
			  1 << DPHY_ANA3_REG_ENLDOR_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOD,
			  1 << DPHY_ANA3_REG_ENLDOD_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOC,
			  1 << DPHY_ANA3_REG_ENLDOC_SHIFT);
	udelay(3);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENVTTC,
			  0 << DPHY_ANA3_REG_ENVTTC_SHIFT);
}

void sun6i_dphy_ana_exit(struct regmap *regmap)
{
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENVTTC,
			  1 << DPHY_ANA3_REG_ENVTTC_SHIFT);
	udelay(3);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOC,
			  0 << DPHY_ANA3_REG_ENLDOC_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOD,
			  0 << DPHY_ANA3_REG_ENLDOD_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA3_REG, DPHY_ANA3_REG_ENLDOR,
			  0 << DPHY_ANA3_REG_ENLDOR_SHIFT);
	regmap_write_bits(regmap, DPHY_ANA2_REG, DPHY_ANA2_REG_ENIB,
			  0 << DPHY_ANA2_REG_ENIB_SHIFT);
}

unsigned int sun6i_dphy_det_mipi_clk(struct regmap *regmap,
				     unsigned int mipi_bps)
{
	unsigned int freq_cnt;

	sun6i_dphy_rx_freq_cnt_enable(regmap);
	usleep_range(1000, 2000);
	freq_cnt = sun6i_dphy_rx_get_freq_cnt(regmap);
	if (freq_cnt == 0)
		return mipi_bps;
	return 1000 * 8 * (DPHY_CLK / freq_cnt);
}

void sun6i_dphy_set_timing(struct regmap *regmap, unsigned int mipi_bps)
{
	unsigned int rx_dly;
	unsigned int lprst_dly;

#if 0
		mipi_bps = sun6i_dphy_det_mipi_clk(regmap, mipi_bps);
#endif

	rx_dly = mipi_bps == 0 ? 0 : (8 * (DPHY_CLK / (mipi_bps / 8)));
	lprst_dly = mipi_bps == 0 ? 0 : (4 * (DPHY_CLK / (mipi_bps / 2)));

	sun6i_dphy_rx_dbc_enable(regmap);
	sun6i_dphy_rx_set_rx_dly(regmap, rx_dly);
	sun6i_dphy_rx_set_lprst_dly(regmap, lprst_dly);
	sun6i_dphy_rx_hs_clk_miss_cnt_disable(regmap);
	sun6i_dphy_rx_hs_sync_cnt_disable(regmap);
	sun6i_dphy_rx_lp_to_cnt_disable(regmap);
	sun6i_dphy_rx_set_hs_sync_to(regmap, 0xff);
	sun6i_dphy_rx_set_hs_clk_miss(regmap, 0xff);
	sun6i_dphy_rx_set_lp_to(regmap, 0xff);
	sun6i_dphy_rx_set_lp_ulps_wp(regmap, 0xff);
	sun6i_dphy_rx_set_entm_to_enrx_dly(regmap, 4);
}

void sun6i_dphy_set_param(struct sun6i_csi_dev *sdev,
			  struct sun6i_dphy_param *param)
{
	sun6i_dphy_ana_init(sdev->regmap);
	sun6i_dphy_set_timing(sdev->regmap, param->bps);
	sun6i_dphy_rx_disable(sdev->regmap);
	sun6i_dphy_set_data_lane(sdev->regmap, param->lane_num);
	sun6i_dphy_rx_enable(sdev->regmap, param->lane_num);
}

void sun6i_dphy_enable(struct sun6i_csi_dev *sdev)
{
	clk_set_rate(sdev->clk_dphy, DPHY_CLK);
	if (clk_prepare_enable(sdev->clk_dphy)) {
		dev_err(sdev->dev, "Failed to enable DPHY clock");
	}
	regmap_write_bits(sdev->regmap, DPHY_CTL_REG, DPHY_CTL_REG_EN,
			  1 << DPHY_CTL_REG_EN_SHIFT);
}

void sun6i_dphy_disable(struct sun6i_csi_dev *sdev)
{
	regmap_write_bits(sdev->regmap, DPHY_CTL_REG, DPHY_CTL_REG_EN,
			  0 << DPHY_CTL_REG_EN_SHIFT);
	clk_disable_unprepare(sdev->clk_dphy);
	sun6i_dphy_ana_exit(sdev->regmap);
}