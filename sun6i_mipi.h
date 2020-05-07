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

void sun6i_mipi_set_stream(struct sun6i_csi *csi, bool enable);
void sun6i_mipi_setup_bus(struct sun6i_csi *csi);

#endif /* __SUN6I_MIPI_H__ */