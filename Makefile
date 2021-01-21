#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=babeld-ids
PKG_RELEASE:=1

PKG_MAINTAINER:=Nick Hainke <vincent@systemli.org>

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/babeld-ids
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Babel Intrusion Detection
  URL:=https://github.com/PolynomialDivision/babeld-ids.git
  DEPENDS:=+libubus +libubox +libblobmsg-json +libuci +libgcrypt +libiwinfo +umdns +libowipcalc
endef

define Package/babeld-ids/description
  This package implements a decentralized wireless daemon.
endef

define Package/babeld-ids/conffiles
endef

define Package/babeld-ids/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/babeld-ids $(1)/usr/sbin/babeld-ids
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./config/babeld-ids.config $(1)/etc/config/babeld-ids
endef

$(eval $(call BuildPackage,babeld-ids))
