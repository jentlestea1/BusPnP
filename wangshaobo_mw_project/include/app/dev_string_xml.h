/** 
* @file        dev_string_xml.h
* @brief    xml文件的字符串
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef DEV_STRING_XML_H
#define DEV_STRING_XML_H

char xml_string[]={"\
<?xml version=\"1.0\" encoding=\"utf-8\"?>\
<machine architecture=\"sparc\" >\
	<device_description>\
		<device_entity type=\"XM\" lid=\"001\">\
			<description>\
				<interface>i2c</interface>	\
				<model>XM1</model>\
				<dataSize>1</dataSize>\
			</description>\
			<operation>\
				<global name=\"global\">\
					<para_list length=\"1\" name=\"global\">\
						<para name=\"slave_address\" type=\"char\">0xD2</para>\
					</para_list>\
				</global>\
				<op_list length=\"2\">\
					<op name=\"gyroscope_open\" template_id=\"0\">\
						<para_list length=\"5\" name=\"open_template0\">\
							<para address=\"0x20\">0x4F</para>\
							<para address=\"0x21\">0x00</para>\
							<para address=\"0x22\">0x08</para>\
							<para address=\"0x23\">0x00</para>\
							<para address=\"0x24\">0x00</para>\
						</para_list>\
					</op>\
					<op name=\"gyroscope_getx\" template_id=\"0\">\
						<para_list length=\"2\" name=\"getx_template0\">\
							<para name=\"reg_address\" type=\"char\">0x28</para> \
							<para name=\"size\" type=\"int\">2</para>\
						</para_list>\
					</op>\
				</op_list>\
			</operation>\
			<dependence>i2c:003;channel:2</dependence>\
		</device_entity>\
		<device_entity type=\"XM\" lid=\"002\">\
			<description>\
				<interface>uart</interface>\
				<model>XM</model>\
				<dataSize>1</dataSize>\
				<voltage>2.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
		<device_entity type=\"XM\" lid=\"003\">\
			<description>\
				<interface>ad</interface>\
				<model>XM</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>ad:002</dependence>\
		</device_entity>\
		<device_entity type=\"DY\" lid=\"004\">\
			<description>\
				<interface>uart</interface>\
				<model>DY</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
		<device_entity type=\"TLY\" lid=\"005\">\
			<description>\
				<interface>uart</interface>\
				<model>TLY</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
		<device_entity type=\"TLY\" lid=\"006\">\
			<description>\
				<interface>uart</interface>\
				<model>TLY</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
		<device_entity type=\"TLY\" lid=\"007\">\
			<description>\
				<interface>uart</interface>\
				<model>TLY</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
		<device_entity type=\"TLY_C\" lid=\"008\">\
			<description>\
				<interface>uart</interface>\
				<model>TLY_C_01</model>\
				<dataSize>1</dataSize>\
				<voltage>3.0v</voltage>\
			</description>\
			<dependence>uart:007</dependence>\
		</device_entity>\
	</device_description>\
	<IO_description>\
		<bus>\
			<io_i2c_entity type=\"i2c\" lid=\"003\">\
				<channel lid=\"2\">\
					<description/>\
				</channel>\
			</io_i2c_entity>\
			<io_1553b_entity type=\"1553b\" lid=\"001\">\
				<description/>\
			</io_1553b_entity>\
			<io_uart_entity type=\"uart\" lid=\"007\">\
				<description/>\
			</io_uart_entity>\
		</bus>\
		<device_io>\
			<io_ad_entity type=\"ad\" lid=\"002\">\
				<description/>\
			</io_ad_entity>\
		</device_io>\
		<RT>\
			<io_RT_entity type=\"RT\" lid=\"02\">\
				<description>\
					<dev_map_subAddr_list>\
						<device lid=\"001\">1:9000:8001</device>\
						<device lid=\"002\">1:9000:8002</device>\
						<device lid=\"003\">1:9000:8003</device>\
						<device lid=\"004\">1:9000:8004</device>\
						<device lid=\"005\">1:9000:8005</device>\
						<device lid=\"006\">1:9000:8006</device>\
						<device lid=\"007\">1:9000:8007</device>\
						<device lid=\"008\">1:9000:8008</device>\
					</dev_map_subAddr_list>\
				</description>\
				<tranData_description>\
					<send_priority>\
						<device lid=\"001\">100</device>\
                        <device lid=\"002\">200</device>\
						<device lid=\"003\">300</device>\
						<device lid=\"004\">400</device>\
						<device lid=\"005\">500</device>\
						<device lid=\"006\">600</device>\
						<device lid=\"007\">700</device>\
						<device lid=\"008\">800</device>\
					</send_priority>\
					<send_blockSize>\
						<device lid=\"001\">3</device>\
						<device lid=\"002\">3</device>\
						<device lid=\"003\">3</device>\
						<device lid=\"004\">1</device>\
						<device lid=\"005\">1</device>\
						<device lid=\"006\">1</device>\
						<device lid=\"007\">1</device>\
						<device lid=\"008\">1</device>\
					</send_blockSize>\
					<receive_priority>\
						<device lid=\"001\">100</device>\
						<device lid=\"002\">200</device>\
						<device lid=\"003\">300</device>\
						<device lid=\"004\">400</device>\
						<device lid=\"005\">500</device>\
						<device lid=\"006\">600</device>\
						<device lid=\"007\">700</device>\
						<device lid=\"008\">800</device>\
					</receive_priority>\
					<receive_blockSize>\
						<device lid=\"001\">1</device>\
						<device lid=\"002\">1</device>\
						<device lid=\"003\">3</device>\
						<device lid=\"004\">1</device>\
						<device lid=\"005\">1</device>\
						<device lid=\"006\">1</device>\
						<device lid=\"007\">1</device>\
						<device lid=\"008\">1</device>\
					</receive_blockSize>\
				</tranData_description>\
			</io_RT_entity>\
			<io_RT_entity type=\"RT\" lid=\"04\">\
				<description>\
                    <dev_map_subAddr_list/>\
				</description>\
				<tranData_description>\
					<send_priority>\
					</send_priority>\
					<send_blockSize>\
					</send_blockSize>\
					<receive_priority>\
					</receive_priority>\
					<receive_blockSize>\
					</receive_blockSize>\
				</tranData_description>\
			</io_RT_entity>\
		</RT>\
	</IO_description>\
	<config_description>\
		<io_i2c_config lid=\"003\" channel=\"2\">\
			<parent>RT:02</parent>\
		</io_i2c_config>\
		<io_RT_config lid=\"02\">\
			<parent>1553b:001</parent>\
		</io_RT_config>\
		<io_1553b_config lid=\"001\">\
			<parent>ROOT</parent>\
		</io_1553b_config>\
		<io_uart_config lid=\"007\">\
			<parent>RT:02</parent>\
		</io_uart_config>\
		<io_RT_config lid=\"04\">\
			<parent>1553b:001</parent>\
		</io_RT_config>\
		<io_ad_config lid=\"002\">\
			<parent>RT:02</parent>\
		</io_ad_config>\
	</config_description>\
</machine>\
"};

#endif

