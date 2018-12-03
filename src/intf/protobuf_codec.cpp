
#include "protobuf_codec.h"
#include "protocol.pb.h"
#include <iostream>
#include <sstream>

#include "Logger.h"

using namespace tutorial;

iEvent* protobuf_protocol_codec_t::decode(u16 mid, u8* buffer, u32 size)
{
    iEvent* ev= NULL;

    switch (mid )
    {
    case EEVENTID_GET_MOBILE_CODE_REQ:
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
    case EEVENTID_LIST_ACCOUNT_RECORDS_REQ:
        ev = decode_2_list_account_record_req_ev(buffer, size);
        break;
    case EEVENTID_UNLOCK_REQ:
        ev = decode_2_unlock_req_ev(attributes);
        break;
    case EEVENTID_LIST_TRAVELS_REQ:
        ev = decode_2_list_travel_records_req_ev(buffer, size);
        break;
    case EEVENTID_LOCK_REQ:
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
    switch(ev->get_eid())
    {
    case EEVENTID_GET_MOBILE_CODE_RSP:
        ret = encode_common_rsp_ev(ev, buffer, size);
        break;
    case :
        
    }

    return ret;
}

MobileCodeReqEv* protobuf_protocol_codec_t::decode_2_mobile_code_req_ev(const  u8* buffer, u32 size)
{
    mobile_request mobile_req;
    std::istringstream ss;
    ss.rdbuf()->pubsetbuf(buffer, size);
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
    ss.rdbuf()->pubsetbuf(buffer, size)
    login_request.ParseFromIstream(&ss);

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
    LoginReqEv* ev = new LoginReqEv(login_req.mobile(), login_req.code());

    return ev;
}


RechargeReqEv* protobuf_protocol_codec_t::decode_2_recharge_req_ev(const u8* buffer, u32 size)
{
    recharge_request recharge_req;
    if (!recharge_req.has_mobile() || !recharge_req.has_amount() )
    {
        return NULL;
    }

    RechargeReqEv* ev = new RechargeReqEv(recharge_req.mobile(), recharge_req.amount());

    return ev;
}


bool protobuf_protocol_codec_t::encode_common_rsp_ev(CommonRspEv* rsp, u8* buffer, u32 size)
{
    rsponse_result rsp_ret;
    rsp_ret.set_code(rsp->get_code());
    rsp_ret.set_msg(rsp->get_msg());
    if (!rsp->get_data().empty())
    {
        rsp_ret.set_data(rsp->get_data());
    }

    std::ostringstream ss;
    ss.rdbuf()->pubsetbuf(buffer, size);

    return rsp_ret.SerializeToOstream(&ss);
}


