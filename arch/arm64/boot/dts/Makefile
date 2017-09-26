dts-dirs += apm
dts-dirs += arm
dts-dirs += cavium

DTS_DIR := qcom

ifeq ($(CONFIG_PROJECT_P7201),y)
DTS_DIR := p7201
endif

dts-dirs += $(DTS_DIR)

subdir-y	:= $(dts-dirs)
