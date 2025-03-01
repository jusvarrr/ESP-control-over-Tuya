include $(TOPDIR)/rules.mk

PKG_NAME:=tuyaespcontrol
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	CATEGORY:=Base system
	TITLE:=tuyaespcontrol
	DEPENDS:=+libtuyasdk +libubox +libubus +USE_UCLIBC:argp-standalone +libserialport +libblobmsg-json
endef

define Package/$(PKG_NAME)/description
    Tuya ESP Control using Tuya SDK and libserialport Library
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuyaespcontrol $(1)/usr/bin
	$(INSTALL_BIN) ./files/tuyaespcontrol.init $(1)/etc/init.d/tuyaespcontrol
	$(INSTALL_CONF) ./files/tuyaespcontrol.config $(1)/etc/config/tuyaespcontrol
endef

$(eval $(call BuildPackage,tuyaespcontrol))