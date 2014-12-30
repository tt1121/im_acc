include $(TOPDIR)/rules.mk

PKG_NAME:=wideband_speedup
PKG_RELEASE:=1.0
PKG_VERSION:=1.0

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/wideband_speedup
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=update the package and firmware
  DEPENDS := +libcurl +libpolarssl +libmsg +libjson-c +libpthread
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include 
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)  -lmsg -lpolarssl -lcurl -ljson-c -lpthread" \
	wideband_speedup
endef

define Package/wideband_speedup/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wideband_speedup $(1)/bin
endef

$(eval $(call BuildPackage,wideband_speedup))
