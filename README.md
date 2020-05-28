# Allwinner CSI Driver with DVP & MIPI-CSI2 for Mainline Linux 5.4
### CSI driver based on Mainline SUN6I-CSI

## Install

### Put all file in "drivers/staging/media/sunxi/sun6i-csi"

### Add source to "drivers/staging/media/sunxi/Kconfig"
```
source "drivers/staging/media/sunxi/sun6i-csi/Kconfig"
```
Demo
```
# SPDX-License-Identifier: GPL-2.0
config VIDEO_SUNXI
	bool "Allwinner sunXi family Video Devices"
	depends on ARCH_SUNXI || COMPILE_TEST
	help
	  If you have an Allwinner SoC based on the sunXi family, say Y.

	  Note that this option doesn't include new drivers in the
	  kernel: saying N will just cause Kconfig to skip all the
	  questions about Allwinner media devices.

if VIDEO_SUNXI

source "drivers/staging/media/sunxi/cedrus/Kconfig"
source "drivers/staging/media/sunxi/sun6i-csi/Kconfig"

endif
```

### Add obj to "drivers/staging/media/sunxi/Makefile"
```
obj-$(CONFIG_VIDEO_SUN6I_MIPI_CSI) += sun6i-csi/
```
Demo
```
# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_VIDEO_SUNXI_CEDRUS)	+= cedrus/
obj-$(CONFIG_VIDEO_SUN6I_MIPI_CSI) += sun6i-csi/
```

## DeviceTree
OV5640 Demo for Allwinner V3 / V3s / S3L / S3
### .dtsi for SoC
```
csi0: csi@01cb0000 {
    compatible = "allwinner,sun8i-v3s-csi";
    reg = <0x01cb0000 0x3000>;
    interrupts = <GIC_SPI 83 IRQ_TYPE_LEVEL_HIGH>;
    clocks = <&ccu CLK_BUS_CSI>, <&ccu CLK_CSI1_SCLK>, <&ccu CLK_DRAM_CSI>, <&ccu CLK_MIPI_CSI>;
    clock-names = "bus", "mod", "ram", "dphy";
    resets = <&ccu RST_BUS_CSI>;
    status = "disabled";
};

csi1: csi@1cb4000 {
    compatible = "allwinner,sun8i-v3s-csi";
    reg = <0x01cb4000 0x3000>;
    interrupts = <GIC_SPI 84 IRQ_TYPE_LEVEL_HIGH>;
    clocks = <&ccu CLK_BUS_CSI>, <&ccu CLK_CSI1_SCLK>, <&ccu CLK_DRAM_CSI>;
    clock-names = "bus", "mod", "ram";
    resets = <&ccu RST_BUS_CSI>;
    status = "disabled";
};
```
### .dts for board
```
&csi0 {
    status = "okay";
    allwinner,mipi-csi-bps = <400000000>;
    port {
        csi0_ep: endpoint {
            remote-endpoint = <&ov5640_0>;
            clock-lanes = <0>;
            data-lanes = <1 2>;
        };
    };
};

&i2c1 {
    status = "okay";
    ov5640: camera@3c {
        compatible = "ovti,ov5640";
        reg = <0x3c>;
        pinctrl-names = "default";
        pinctrl-0 = <&csi0_mclk>;
        clocks = <&ccu CLK_CSI0_MCLK>;
        clock-names = "xclk";
        assigned-clocks = <&ccu CLK_CSI0_MCLK>;
        assigned-clock-rates = <24000000>;
        powerdown-gpios = <&pio 4 19 GPIO_ACTIVE_HIGH>;
        reset-gpios = <&pio 4 18 GPIO_ACTIVE_LOW>;
        port {
            ov5640_0: endpoint {
                remote-endpoint = <&csi0_ep>;
                clock-lanes = <0>;
                data-lanes = <1 2>;
            };
        };
    };
};

&pio {
    csi0_mclk: csi0-mclk@0 {
        pins = "PE20";
        function = "csi_mipi";
    };
};
```
## Compile
### Enable Driver in 
```
> Device Drivers > Staging drivers > Media staging drivers
[*]   Allwinner sunXi family Video Devices
<*>   Allwinner V3s Camera Sensor Interface driver with MIPI-CSI2 support
```
... and here we go.

## Test
```
media-ctl --set-v4l2 '5:0[fmt:UYVY8_2X8/1920x1080@1/15]'
fswebcam -S 5 -d /dev/video0 -p YUV420P -r 1920x1080 test.jpg
```

## Debug
### Default
Report in issue.

## Need HW Acceleration Encoding?
CedarX Driver for Mainline (https://github.com/aodzip/cedar)
