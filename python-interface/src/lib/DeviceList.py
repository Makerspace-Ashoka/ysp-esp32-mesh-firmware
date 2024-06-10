'''
This file contains the list of ESPs allowed to be used for YSP workshop in form of a python dictionary with 'NodeID' as key and 'HardwareID' as value.
The ESPs are identified by their NodeID, which is a unique identifier for each ESP.
The NodeID is derived by converting last 32bits of the ESP's MAC address to base 10 (Decimal/int).
'''
AllowedDevicesNodeIDs: dict[int, int] = {
    864076657: 0,
    864076913: 1,
    2365952225: 2,
    864074501: 3,
    864072381: 4,
    3867413669: 5,
    864041801: 6,
    2365948837: 7,
    864069593: 8,
    864073565: 9,
    2365949985: 10,
    864077329: 11,
    3867414565: 12,
    864076945: 13,
    3867413857: 14,
    2365948089: 15,
    3867413293: 16,
    2365952577: 17,
    2365952329: 18,
    3867421645: 19,
    864040541: 20,
    864077385: 21,
    3867412793: 22,
    2365951949: 23,
    3867411913: 24,
    2365951113: 25,
    864073801: 26,
    3867413889: 27,
    3867411781: 28,
    3867410537: 29,
    3867413665: 30,
    864065333: 31,
    864060077: 32,
    864061849: 33,
    3867410645: 34,
    864070781: 35,
    2365947425: 36,
    864072973: 37,
    2365949265: 38,
    2365947517: 39,
    3867410601: 40,
    864060597: 41,
    3867413809: 42,
    864072769: 43,
    2365949373: 44,
    2365944413: 45,
    2365950521: 46,
    3867414557: 47,
    2365951497: 48,
    864069165: 49,
    2365952357: 50,
    864065361: 51,
    2365951869: 52,
    864060033: 53,
    864060877: 54,
    2365949461: 55,
    2365947113: 56,
    3867412585: 57,
    2365947765: 58,
    3867413037: 59,
    2365952105: 60,
    864071897: 61,
    2365952397: 62,
    2365946201: 63,
    864075961: 64,
    3867414265: 65,
    2365951697: 66,
    2365952661: 67,
    2365950961: 68,
    864063689: 69,
    3867411805: 70,
    2365944825: 71,
    2365952805: 72,
    2365949769: 73,
    864060237: 74,
    2365951381: 75,
    864070425: 76
}