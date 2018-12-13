
#include "json_protocol_codec.h"
#include "Logger.h"

#include <json/json.h>
#include <string.h>

bool json_protocol_codec_t::encode(iEvent* ev, u8* buffer, u32 size)
{
    bool ret = false;
    u32 eid = ev->get_eid();
    switch(eid)
    {
    case EEVENTID_GET_MOBILE_CODE_RSP://CommonRspEv
        ret = encode_common_rsp_ev(dynamic_cast<CommonRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_LOGIN_RSP://CommonRspEv
        ret = encode_common_rsp_ev(dynamic_cast<CommonRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_RECHARGE_RSP://CommonRspEv
        ret = encode_common_rsp_ev(dynamic_cast<CommonRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_GET_ACCOUNT_BALANCE_RSP://GetAccountBalanceRspEv ok
        ret = encode_get_account_balance_rsp_ev(dynamic_cast<GetAccountBalanceRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_LIST_ACCOUNT_RECORDS_RSP://ListAccountRecordsRspEv
        ret = encode_list_account_records_rsp_ev(dynamic_cast<ListAccountRecordsRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_UNLOCK_RSP://UnlockRspEv
        ret = encode_common_rsp_ev(dynamic_cast<CommonRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_LIST_TRAVELS_RSP://ListTravelRecordsRspEv
        ret= encode_list_travels_records_rsp_ev(dynamic_cast<ListTravelRecordsRspEv *>(ev), buffer, size);
        break;
    case EEVENTID_LOCK_RSP://LockRspEv
        ret = encode_common_rsp_ev(dynamic_cast<CommonRspEv *>(ev), buffer, size);
        break;
    default:
        LOG_WARN("mid %d is invalid.", eid);
        break;          
   }

    return ret;
}


iEvent* json_protocol_codec_t::decode(u16 mid, u8* buffer, u32 size)
{
    Json::Reader reader;
    Json::Value attributes;
    if (!reader.parse((const char*)buffer, attributes))
        return NULL;

    iEvent* ev= NULL;

    switch (mid )
    {
    case EEVENTID_GET_MOBILE_CODE_REQ:
        ev = decode_2_mobile_code_req_ev(attributes);
        break;
    case EEVENTID_LOGIN_REQ:
        ev = decode_2_login_req_ev(attributes);
        break;
    case EEVENTID_RECHARGE_REQ:
        ev = decode_2_recharge_req_ev(attributes);
        break;
    case EEVENTID_GET_ACCOUNT_BALANCE_REQ:
        ev = decode_2_get_account_balance_req_ev(attributes);
        break;
    case EEVENTID_LIST_ACCOUNT_RECORDS_REQ:
        ev = decode_2_list_account_record_req_ev(attributes);
        break;
    case EEVENTID_UNLOCK_REQ:
        ev = decode_2_unlock_req_ev(attributes);
        break;
    case EEVENTID_LIST_TRAVELS_REQ:
        ev = decode_2_list_travel_records_req_ev(attributes);
        break;
    case EEVENTID_LOCK_REQ:
        ev = decode_2_lock_req_ev(attributes);
        break;
    default:
        LOG_WARN("mid %d is invalid.", mid);
        break;
    }

    return ev;
}

MobileCodeReqEv* json_protocol_codec_t::decode_2_mobile_code_req_ev(const Json::Value& attributes)
{
    if (attributes["mobile"] == Json::nullValue)
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();

    MobileCodeReqEv* ev = new MobileCodeReqEv(mobile);

    return ev;
}

LoginReqEv* json_protocol_codec_t::decode_2_login_req_ev(const Json::Value& attributes)
{
    if ((attributes["mobile"] == Json::nullValue) || (attributes["code"] == Json::nullValue))
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();
    std::string code   = attributes["code"].asString();
    LoginReqEv* ev = new LoginReqEv(mobile, code);

    return ev;
}

RechargeReqEv* json_protocol_codec_t::decode_2_recharge_req_ev(const Json::Value& attributes)
{
    if ((attributes["mobile"] == Json::nullValue) || (attributes["amount"] == Json::nullValue))
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();
    int amount = attributes["amount"].asInt();
    RechargeReqEv* ev = new RechargeReqEv(mobile, amount);

    return ev;
}

GetAccountBalanceReqEv* json_protocol_codec_t::decode_2_get_account_balance_req_ev(const Json::Value& attributes)
{
    if (attributes["mobile"] == Json::nullValue)
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();
    GetAccountBalanceReqEv* ev = new GetAccountBalanceReqEv(mobile);

    return ev;
}

UnlockReqEv* json_protocol_codec_t::decode_2_unlock_req_ev(const Json::Value& attributes)
{
    if ((attributes["mobile"] == Json::nullValue) || (attributes["bikecode"] == Json::nullValue))
    {
        return NULL;
    }

    std::string mobile   = attributes["mobile"].asString();
    std::string bikecode = attributes["bikecode"].asString();

    return new UnlockReqEv(mobile, bikecode);
}

LockReqEv* json_protocol_codec_t::decode_2_lock_req_ev(const Json::Value& attributes)
{
    if ((attributes["mobile"] == Json::nullValue) || (attributes["bikecode"] == Json::nullValue))
    {
        return NULL;
    }

    std::string mobile   = attributes["mobile"].asString();
    std::string bikecode = attributes["bikecode"].asString();

    return new LockReqEv(mobile, bikecode);
}

ListAccountRecordsReqEv* json_protocol_codec_t::decode_2_list_account_record_req_ev(const Json::Value& attributes)
{
    if (attributes["mobile"] == Json::nullValue)
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();
    return new ListAccountRecordsReqEv(mobile);
}

ListTravelRecordsReqEv* json_protocol_codec_t::decode_2_list_travel_records_req_ev(const Json::Value& attributes)
{
    if (attributes["mobile"] == Json::nullValue)
    {
        return NULL;
    }

    std::string mobile = attributes["mobile"].asString();
    return new ListTravelRecordsReqEv(mobile);
}

bool json_protocol_codec_t::encode_common_rsp_ev(CommonRspEv* rsp, u8* buffer, u32 size)
{
    Json::Value attributes;
    attributes["code"] = rsp->get_code();
    attributes["msg"]  = rsp->get_msg();
    attributes["data"] = rsp->get_data();
    Json::FastWriter writer;
    std::string body = writer.write(attributes);
    memcpy(buffer, body.data(), body.size());

    return true;
}

bool json_protocol_codec_t::encode_get_account_balance_rsp_ev(GetAccountBalanceRspEv* rsp, u8* buffer, u32 size)
{
    Json::Value attributes;
    attributes["code"] = rsp->get_code();
    attributes["msg"]  = rsp->get_msg();
    attributes["data"] = rsp->get_data();
    attributes["balance"] = rsp->get_balance();

    Json::FastWriter writer;
    std::string body = writer.write(attributes);
    memcpy(buffer, body.data(), body.size());

    return true;
}


bool json_protocol_codec_t::encode_list_account_records_rsp_ev(ListAccountRecordsRspEv* rsp, u8* buffer, u32 size)
{
    Json::Value attributes;
    attributes["code"] = rsp->get_code();
    attributes["msg"]  = rsp->get_msg();
    attributes["data"] = rsp->get_data();    

    Json::Value rsp_ret;
    rsp_ret["ret"] = attributes;
    
    //array
    const std::vector<AccountRecord> &recodes = rsp->get_records();    
    for (size_t pos = 0 ; pos < recodes.size();++pos)
    {
        Json::Value record;
        record["type"]      = recodes[pos].type;
        record["limit"]     = recodes[pos].limit;
        //TODO: longlong
        //record["timestamp"] = recodes[pos].timestamp;
        rsp_ret["records"].append(record);
    }
    
    Json::FastWriter writer;
    std::string body = writer.write(rsp_ret);
    memcpy(buffer, body.data(), body.size());

    return true;
}


bool json_protocol_codec_t::encode_list_travels_records_rsp_ev(ListTravelRecordsRspEv* rsp, u8* buffer, u32 size)   
{
    Json::Value attributes;
    attributes["code"] = rsp->get_code();
    attributes["msg"]  = rsp->get_msg();
    attributes["data"] = rsp->get_data();    

    Json::Value rsp_ret;
    rsp_ret["ret"] = attributes;
    rsp_ret["mileage"]   = rsp->get_travel_info().mileage;
    rsp_ret["discharge"] = rsp->get_travel_info().discharge;
    rsp_ret["calorie"]   = rsp->get_travel_info().calorie;
    //array
    const std::vector<TravelRecord> &recodes = rsp->get_travel_info().records;    
    for (size_t pos = 0 ; pos < recodes.size();++pos)
    {
        Json::Value record;
        record["amount"]   = recodes[pos].amount;
        record["duration"] = recodes[pos].duration;
        //TODO:longlong
        //record["stm"]      = recodes[pos].startTimeStamp;
    }   
    
    Json::FastWriter writer;
    std::string body = writer.write(rsp_ret);
    memcpy(buffer, body.data(), body.size());     

    return true;
}
