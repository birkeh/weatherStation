#include "cmasterbrick.h"
#include "ctinkerforge.h"


cExtension::cExtension(const quint32& exttype, const QString& name) :
	m_exttype(exttype),
	m_name(name)
{
}

quint32 cExtension::exttype()
{
	return(m_exttype);
}

QString cExtension::name()
{
	return(m_name);
}

cExtension* cExtensionList::add(const quint32& exttype, const QString& name)
{
	cExtension*	lpNew	= find(exttype, name);

	if(!lpNew)
	{
		lpNew	= new cExtension(exttype, name);
		append(lpNew);
	}
	return(lpNew);
}

cExtension* cExtensionList::find(const quint32& exttype, const QString& name)
{
	for(int x = 0;x < count();x++)
	{
		if(this->at(x)->exttype() == exttype && this->at(x)->name() == name)
			return(this->at(x));
	}
	return(nullptr);
}

cExtension* cExtensionList::find(const quint32& exttype)
{
	for(int x = 0;x < count();x++)
	{
		if(this->at(x)->exttype() == exttype)
			return(this->at(x));
	}
	return(nullptr);
}

cExtension* cExtensionList::find(const QString& name)
{
	for(int x = 0;x < count();x++)
	{
		if(this->at(x)->name() == name)
			return(this->at(x));
	}
	return(nullptr);
}

cMasterBrick::cMasterBrick(cTinkerForge *tinkerForge, const QString &uid, QObject *parent) :
	QObject(parent),
	m_tinkerForge(tinkerForge),
	m_uid(uid),
	m_lastError(0)
{
	m_extensionList.add(1, "Chibi");
	m_extensionList.add(2, "RS485");
	m_extensionList.add(3, "WIFI");
	m_extensionList.add(4, "Ethernet");
	m_extensionList.add(5, "WIFI 2.0");

	if(!m_tinkerForge)
		return;

	master_create(&m_master, m_uid.toLocal8Bit().data(), m_tinkerForge->connection());
}

cMasterBrick::~cMasterBrick()
{
	master_destroy(&m_master);
}

qint32 cMasterBrick::collect()
{
	uint8_t		api_version[3];
	uint16_t	voltage;
	uint16_t	current;
	uint32_t	debounce;
	bool		bricklets_enabled;
	uint8_t		communication_method;
	uint32_t	timeout_count;
	bool		enable_dynamic_baudrate;
	uint32_t	minimum_dynamic_baudrate;
	char		bricklet_port;
	uint32_t	baudrate;

	m_lastError	= 0;

	if(master_get_api_version(&m_master, api_version) >= 0)
		m_apiVersion	= QString("%1.%2.%3").arg(api_version[0]).arg(api_version[1]).arg(api_version[2]);
	else
		m_lastError	|= API_VERSION;

	if(master_get_stack_voltage(&m_master, &voltage) >= 0)
		m_voltage	= (double)voltage/100.0;
	else
		m_lastError	|= VOLTAGE;

	if(master_get_stack_current(&m_master, &current) != 0)
		m_current	= (double)current/100.0;
	else
		m_lastError	|= CURRENT;

	if(master_get_usb_voltage(&m_master, &voltage) >= 0)
		m_usbVoltage	= (double)voltage/100.0;
	else
		m_lastError	|= USB_VOLTAGE;

	if(master_get_debounce_period(&m_master, &debounce) >= 0)
		m_debouncePeriod	= debounce;
	else
		m_lastError	|= DEBOUNCE_PERIOD;

	if(master_get_bricklets_enabled(&m_master, &bricklets_enabled) >= 0)
		m_brickletsEnabled	= bricklets_enabled;
	else
		m_lastError	|= BRICKLETS_ENABLED;

	for(communication_method = 0;communication_method < 8;communication_method++)
	{
		if(master_get_send_timeout_count(&m_master, communication_method, &timeout_count) >= 0)
			m_timeoutCount[communication_method]	= timeout_count;
		else
		{
			m_lastError	|= COMMUNICATION_METHOD;
			m_lastError	|= TIMEOUT_COUNT;
		}
	}

	if(master_get_spitfp_baudrate_config(&m_master, &enable_dynamic_baudrate, &minimum_dynamic_baudrate) >= 0)
	{
		m_enableDynamicBaudrate		= enable_dynamic_baudrate;
		m_minimumDynamicBaudrate	= minimum_dynamic_baudrate;
	}
	else
	{
		m_lastError	|= ENABLE_DYNAMIC_BAUDRATE;
		m_lastError	|= MINIMUM_DYNAMIC_BAUDRATE;
	}

	for(bricklet_port = 0;bricklet_port < 5;bricklet_port++)
	{
		if(master_get_spitfp_baudrate(&m_master, bricklet_port, &baudrate) >= 0)
			m_spitfpBaudrate[bricklet_port]	= baudrate;
		else
			m_lastError	|= SPITFP_BAUDRATE;
	}

//int master_get_spitfp_error_count(Master *master, char bricklet_port, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow);
//int master_is_status_led_enabled(Master *master, bool *ret_enabled);
//int master_get_protocol1_bricklet_name(Master *master, char port, uint8_t *ret_protocol_version, uint8_t ret_firmware_version[3], char ret_name[40]);
//int master_get_chip_temperature(Master *master, int16_t *ret_temperature);
//int master_get_identity(Master *master, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier);

//int master_is_chibi_present(Master *master, bool *ret_present);
//int master_get_chibi_address(Master *master, uint8_t *ret_address);
//int master_get_chibi_master_address(Master *master, uint8_t *ret_address);
//int master_get_chibi_slave_address(Master *master, uint8_t num, uint8_t *ret_address);
//int master_get_chibi_signal_strength(Master *master, uint8_t *ret_signal_strength);
//int master_get_chibi_error_log(Master *master, uint16_t *ret_underrun, uint16_t *ret_crc_error, uint16_t *ret_no_ack, uint16_t *ret_overflow);
//int master_get_chibi_frequency(Master *master, uint8_t *ret_frequency);
//int master_get_chibi_channel(Master *master, uint8_t *ret_channel);
//int master_is_rs485_present(Master *master, bool *ret_present);
//int master_get_rs485_address(Master *master, uint8_t *ret_address);
//int master_get_rs485_slave_address(Master *master, uint8_t num, uint8_t *ret_address);
//int master_get_rs485_error_log(Master *master, uint16_t *ret_crc_error);
//int master_get_rs485_configuration(Master *master, uint32_t *ret_speed, char *ret_parity, uint8_t *ret_stopbits);
//int master_is_wifi_present(Master *master, bool *ret_present);
//int master_get_wifi_configuration(Master *master, char ret_ssid[32], uint8_t *ret_connection, uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint16_t *ret_port);
//int master_get_wifi_encryption(Master *master, uint8_t *ret_encryption, char ret_key[50], uint8_t *ret_key_index, uint8_t *ret_eap_options, uint16_t *ret_ca_certificate_length, uint16_t *ret_client_certificate_length, uint16_t *ret_private_key_length);
//int master_get_wifi_status(Master *master, uint8_t ret_mac_address[6], uint8_t ret_bssid[6], uint8_t *ret_channel, int16_t *ret_rssi, uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint32_t *ret_rx_count, uint32_t *ret_tx_count, uint8_t *ret_state);
//int master_get_wifi_certificate(Master *master, uint16_t index, uint8_t ret_data[32], uint8_t *ret_data_length);
//int master_get_wifi_power_mode(Master *master, uint8_t *ret_mode);
//int master_get_wifi_buffer_info(Master *master, uint32_t *ret_overflow, uint16_t *ret_low_watermark, uint16_t *ret_used);
//int master_get_wifi_regulatory_domain(Master *master, uint8_t *ret_domain);
//int master_get_long_wifi_key(Master *master, char ret_key[64]);
//int master_get_wifi_hostname(Master *master, char ret_hostname[16]);
//int master_get_wifi_authentication_secret(Master *master, char ret_secret[64]);
//int master_is_ethernet_present(Master *master, bool *ret_present);
//int master_get_ethernet_configuration(Master *master, uint8_t *ret_connection, uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint16_t *ret_port);
//int master_get_ethernet_status(Master *master, uint8_t ret_mac_address[6], uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint32_t *ret_rx_count, uint32_t *ret_tx_count, char ret_hostname[32]);
//int master_get_ethernet_websocket_configuration(Master *master, uint8_t *ret_sockets, uint16_t *ret_port);
//int master_get_ethernet_authentication_secret(Master *master, char ret_secret[64]);
//int master_get_connection_type(Master *master, uint8_t *ret_connection_type);
//int master_is_wifi2_present(Master *master, bool *ret_present);
//int master_get_wifi2_authentication_secret(Master *master, char ret_secret[64]);
//int master_get_wifi2_configuration(Master *master, uint16_t *ret_port, uint16_t *ret_websocket_port, uint16_t *ret_website_port, uint8_t *ret_phy_mode, uint8_t *ret_sleep_mode, uint8_t *ret_website);
//int master_get_wifi2_status(Master *master, bool *ret_client_enabled, uint8_t *ret_client_status, uint8_t ret_client_ip[4], uint8_t ret_client_subnet_mask[4], uint8_t ret_client_gateway[4], uint8_t ret_client_mac_address[6], uint32_t *ret_client_rx_count, uint32_t *ret_client_tx_count, int8_t *ret_client_rssi, bool *ret_ap_enabled, uint8_t ret_ap_ip[4], uint8_t ret_ap_subnet_mask[4], uint8_t ret_ap_gateway[4], uint8_t ret_ap_mac_address[6], uint32_t *ret_ap_rx_count, uint32_t *ret_ap_tx_count, uint8_t *ret_ap_connected_count);
//int master_get_wifi2_client_configuration(Master *master, bool *ret_enable, char ret_ssid[32], uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint8_t ret_mac_address[6], uint8_t ret_bssid[6]);
//int master_get_wifi2_client_hostname(Master *master, char ret_hostname[32]);
//int master_get_wifi2_client_password(Master *master, char ret_password[64]);
//int master_get_wifi2_ap_configuration(Master *master, bool *ret_enable, char ret_ssid[32], uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint8_t *ret_encryption, bool *ret_hidden, uint8_t *ret_channel, uint8_t ret_mac_address[6]);
//int master_get_wifi2_ap_password(Master *master, char ret_password[64]);
//int master_get_wifi2_firmware_version(Master *master, uint8_t ret_firmware_version[3]);
//int master_is_wifi2_status_led_enabled(Master *master, bool *ret_enabled);
//int master_get_wifi2_mesh_configuration(Master *master, bool *ret_enable, uint8_t ret_root_ip[4], uint8_t ret_root_subnet_mask[4], uint8_t ret_root_gateway[4], uint8_t ret_router_bssid[6], uint8_t ret_group_id[6], char ret_group_ssid_prefix[16], uint8_t ret_gateway_ip[4], uint16_t *ret_gateway_port);
//int master_get_wifi2_mesh_router_ssid(Master *master, char ret_ssid[32]);
//int master_get_wifi2_mesh_router_password(Master *master, char ret_password[64]);
//int master_get_wifi2_mesh_common_status(Master *master, uint8_t *ret_status, bool *ret_root_node, bool *ret_root_candidate, uint16_t *ret_connected_nodes, uint32_t *ret_rx_count, uint32_t *ret_tx_count);
//int master_get_wifi2_mesh_client_status(Master *master, char ret_hostname[32], uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint8_t ret_mac_address[6]);
//int master_get_wifi2_mesh_ap_status(Master *master, char ret_ssid[32], uint8_t ret_ip[4], uint8_t ret_subnet_mask[4], uint8_t ret_gateway[4], uint8_t ret_mac_address[6]);

//int master_get_extension_type(Master *master, uint8_t extension, uint32_t *ret_exttype);

	return(m_lastError);
}

QString cMasterBrick::uid()
{
	return(m_uid);
}

qint32 cMasterBrick::lastError()
{
	return(m_lastError);
}
