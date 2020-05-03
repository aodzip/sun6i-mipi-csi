# SPDX-License-Identifier: GPL-2.0-only
sun6i-csi-y += sun6i_video.o sun6i_csi.o
obj-$(CONFIG_VIDEO_SUN6I_MIPI_CSI) += sun6i-csi.o
sun6i-csi-y	+=	mipi_csi/bsp_common.o mipi_csi/bsp_mipi_csi.o
sun6i-csi-y	+=	mipi_csi/protocol/protocol_reg.o mipi_csi/dphy/dphy_reg.o
sun6i-csi-y	+=	mipi_csi/sunxi_mipi.o