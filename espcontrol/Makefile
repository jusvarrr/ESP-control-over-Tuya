include $(TOPDIR)/rules.mk

PKG_NAME:=espcontrol
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	CATEGORY:=Base system
	TITLE:=ESP8266 control via UBUS
	DEPENDS:=+libserialport +libubox +libubus +libblobmsg-json
endef

define Package/$(PKG_NAME)/description
	A tool to control ESP8266 devices via UBUS.
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/espcontrol $(1)/usr/bin
	$(INSTALL_BIN) ./files/espcontrol.init $(1)/etc/init.d/espcontrol
endef

$(eval $(call BuildPackage,espcontrol))
