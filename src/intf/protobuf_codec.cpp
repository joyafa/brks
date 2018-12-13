
#include "protobuf_codec.h"
#include "protocol.pb.h"
#include <iostream>
#include <sstream>

#include "Logger.h"
using namespace std;
using namespace tutorial;

iEvent* protobuf_protocol_codec_t::decode(u16 mid, u8* buffer, u32 size)
{
    iEvent* ev= NULL;

    switch (mid )
    {
    case EEVENTID_GET_MOBILE_CODE_REQ://ok
        ev = decode_2_mobile_code_req_ev(buffer, size);
        break;
    case EEVENTID_LOGIN_REQ:
        ev = decode_2_login_req_ev(buffer, size);
        break;
    case EEVENTID_RECHARGE_REQ:
        ev = decode_2_recharge_req_ev(buffer, size);
        break;
    case EEVENTID_GET_ACCOUNT_BALANCE_REQ:
        ev = decode_2_get_account_balance_req_ev(buffer, size);
        break;
    case EEVENTID_LIST_ACCOUNT_RECORDS_REQ://ok
        ev = decode_2_list_account_record_req_ev(buffer, size);
        break;
    case EEVENTID_UNLOCK_REQ://ok
        ev = decode_2_unlock_req_ev(buffer, size);
        break;
    case EEVENTID_LIST_TRAVELS_REQ://ok
        ev = decode_2_list_travel_records_req_ev(buffer, size);
        break;
    case EEVENTID_LOCK_REQ://ok
        ev = decode_2_lock_req_ev(buffer, size);
        break;
    default:
        LOG_WARN("mid %d is invalid.", mid);
        break;
    }

    return ev;
}

bool protobuf_protocol_codec_t::encode(iEvent* ev, u8* buffer, u32 size)
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


bool protobuf_protocol_codec_t::encode_list_travels_records_rsp_ev(ListTravelRecordsRspEv* rsp, u8* buffer, u32 size)
{
    list_travel_records_rsponse rsp_ret;
    rsponse_result ret;
    ret.set_code(rsp->get_code());
    ret.set_msg(rsp->get_msg());
    if (!rsp->get_data().empty())
    {
        ret.set_data(rsp->get_data());
    }
    rsp_ret.set_allocated_ret(&ret);
    rsp_ret.set_mileage(rsp->get_travel_info().mileage);
    rsp_ret.set_discharge(rsp->get_travel_info().discharge);
    rsp_ret.set_calorie(rsp->get_travel_info().calorie);
    //array
    const std::vector<TravelRecord> &recodes = rsp->get_travel_info().records;    
    for (size_t pos = 0 ; pos < recodes.size();++pos)
    {
        list_travel_records_rsponse_travel_record* record = rsp_ret.add_records();
        record->set_amount(recodes[pos].amount);
        record->set_duration(recodes[pos].duration);
        record->set_stm(recodes[pos].startTimeStamp);
    }

    return true;
}


bool protobuf_protocol_codec_t::encode_list_account_records_rsp_ev(ListAccountRecordsRspEv* rsp, u8* buffer, u32 size)
{
    list_account_records_response rsp_ret;
    rsponse_result ret;
    ret.set_code(rsp->get_code());
    ret.set_msg(rsp->get_msg());
    if (!rsp->get_data().empty())
    {
        ret.set_data(rsp->get_data());
    }
    rsp_ret.set_allocated_ret(&ret);
    //array
    const std::vector<AccountRecord> &recodes = rsp->get_records();    
    for (size_t pos = 0 ; pos < recodes.size();++pos)
    {
        list_account_records_response_account_record* record = rsp_ret.add_records();
        record->set_type(recodes[pos].type);
        record->set_limit(recodes[pos].limit);
        record->set_timestamp(recodes[pos].timestamp);
    }

    return true;
}


bool protobuf_protocol_codec_t::encode_get_account_balance_rsp_ev(GetAccountBalanceRspEv* rsp, u8* buffer, u32 size)
{
    account_balance_response rsp_ret;
    rsponse_result ret;
    ret.set_code(rsp->get_code());
    ret.set_msg(rsp->get_msg());
    if (!rsp->get_data().empty())
    {
        ret.set_data(rsp->get_data());
    }
    rsp_ret.set_allocated_ret(&ret);
    rsp_ret.set_balance(rsp->get_balance());
    std::ostringstream oss;
    oss.rdbuf()->pubsetbuf((char* )buffer, size);

    return rsp_ret.SerializeToOstream(&oss);
}


MobileCodeReqEv* protobuf_protocol_codec_t::decode_2_mobile_code_req_ev(const  u8* buffer, u32 size)
{
    mobile_request mobile_req;
    std::istringstream ss;
    ss.rdbuf()->pubsetbuf((char* )buffer, size);
    mobile_req.ParseFromIstream(&ss);

    if (!mobile_req.has_mobile())
    {
        LOG_WARN("there is no mobile filed in message mobile_request.");
        return NULL;
    }

    return new MobileCodeReqEv(mobile_req.mobile());
}

LoginReqEv* protobuf_protocol_codec_t::decode_2_login_req_ev(const  u8* buffer, u32 size)
{
    login_request login_req;
    std::istringstream ss;
    ss.rdbuf()->pubsetbuf((char* )buffer, size);
    login_req.ParseFromIstream(&ss);

    if (!login_req.has_mobile())
    {
      LOG_WARN("there is no mobile filed in message login_request.");
        return NULL;
    }  
    if (!login_req.has_code())
    {
      LOG_WARN("there is no code filed in message login_req.");
        return NULL;
    }  
    //TODO: 这个要修改proto文件,改为string
    //i32 code= login_req.code();
    //std::ostringstream oss;
    //oss << login_req.code();
    return new LoginReqEv(login_req.mobile(), login_req.code());
}


RechargeReqEv* protobuf_protocol_codec_t::decode_2_recharge_req_ev(const u8* buffer, u32 size)
{
    recharge_request recharge_req;
    if (!recharge_req.has_mobile() || !recharge_req.has_amount() )
    {
        return NULL;
    }

    return new RechargeReqEv(recharge_req.mobile(), recharge_req.amount());
}


bool protobuf_protocol_codec_t::encode_common_rsp_ev(CommonRspEv* rsp, u8* buffer, u32 size)
{
    rsponse_result rsp_ret;
    std::ostringstream oss;
    
    rsp_ret.set_code(rsp->get_code());
    rsp_ret.set_msg(rsp->get_msg());
    if (!rsp->get_data().empty())
    {
        rsp_ret.set_data(rsp->get_data());
    }
    

    oss.rdbuf()->pubsetbuf((char* )buffer, size);

    return rsp_ret.SerializeToOstream(&oss);
}


LockReqEv* protobuf_protocol_codec_t::decode_2_lock_req_ev(const u8* buffer, u32 size)
{
    lock_request lock_req;
    if (!lock_req.has_mobile() || !lock_req.has_bike_code() )
    {
        return NULL;
    }

    return new LockReqEv(lock_req.mobile(), lock_req.bike_code());
}


UnlockReqEv* protobuf_protocol_codec_t::decode_2_unlock_req_ev(const  u8* buffer, u32 size)
{
    unlock_request unlock_req;
    if (!unlock_req.has_mobile() || !unlock_req.has_bike_code() )
    {
        return NULL;
    }

    return new UnlockReqEv(unlock_req.mobile(), unlock_req.bike_code());
}


ListTravelRecordsReqEv* protobuf_protocol_codec_t::decode_2_list_travel_records_req_ev(const u8* buffer, u32 size)
{
    list_travel_records_request travel_recodes_req;
    if (!travel_recodes_req.has_mobile())
    {
        //TODO: error must have mobile 
        return NULL;
    }
    
    return new ListTravelRecordsReqEv(travel_recodes_req.mobile());
}


ListAccountRecordsReqEv* protobuf_protocol_codec_t::decode_2_list_account_record_req_ev(const u8* buffer, u32 size)
{
    list_account_record_request list_account_record_req;
    if (!list_account_record_req.has_mobile())
    {
        //TODO: error must have mobile 
        return NULL;
    }
    
    return new ListAccountRecordsReqEv(list_account_record_req.mobile());
}


GetAccountBalanceReqEv* protobuf_protocol_codec_t::decode_2_get_account_balance_req_ev(const  u8* buffer, u32 size)
{
    account_balance_request account_balance_req;
    if (!account_balance_req.has_mobile())
    {
        //TODO: error must have mobile 
        return NULL;
    }
    
    return new GetAccountBalanceReqEv(account_balance_req.mobile());
}

