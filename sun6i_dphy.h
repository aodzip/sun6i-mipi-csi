// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020- Aodzip
 * All rights reserved.
 * Author: Aodzip <aodzip@gmail.com>
 */
#ifndef __SUN6I_DPHY_H__
#define __SUN6I_DPHY_H__

#include <linux/regmap.h>
#include "sun6i_csi.h"

struct sun6i_dphy_param {
	unsigned int lane_num;
	unsigned int bps;
};

extern void sun6i_dphy_enable(struct sun6i_csi_dev *sdev);
extern void sun6i_dphy_disable(struct sun6i_csi_dev *sdev);
void sun6i_dphy_set_param(struct sun6i_csi_dev *sdev,
			  struct sun6i_dphy_param *param);

#endif /* __SUN6I_DPHY_H__ */