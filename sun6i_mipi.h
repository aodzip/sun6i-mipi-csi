// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020- Aodzip
 * All rights reserved.
 * Author: Aodzip <aodzip@gmail.com>
 */
#ifndef __SUN6I_MIPI_H__
#define __SUN6I_MIPI_H__

#include <linux/regmap.h>
#include "sun6i_csi.h"

struct sun6i_mipi_param
{
  unsigned int bps;
  struct v4l2_subdev_format fmt;
  struct v4l2_mbus_config cfg;
};

void sun6i_mipi_set_power(struct sun6i_csi *csi, bool enable);
int sun6i_mipi_set_param(struct sun6i_csi *csi, struct sun6i_mipi_param *param);

#endif /* __SUN6I_MIPI_H__ */