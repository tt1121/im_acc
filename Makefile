include $(TOPDIR)/rules.mk

PKG_NAME:=im_acc
PKG_RELEASE:=1.0
PKG_VERSION:=1.0

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/im_acc
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=update the package and firmware
  DEPENDS := +libcurl +libpolarssl +libmsg +libjson-c +libpthread +libuci +libubox +libcrypt +libopenssl
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
	LDFLAGS="$(TARGET_LDFLAGS)  -lmsg -lpolarssl -lcurl -ljson-c -lpthread -luci -lubox -lcrypto" \
	im_acc
endef

define Package/im_acc/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/im_acc $(1)/bin
	$(INSTALL_DIR) $(1)/etc/config/
	$(INSTALL_DATA) ./files/im_acc.config $(1)/etc/config/im_acc
	$(INSTALL_DIR) $(1)/usr/prc_mgt/
	$(INSTALL_DATA) ./files/wideband $(1)/usr/prc_mgt/wideband
endef

$(eval $(call BuildPackage,im_acc))
