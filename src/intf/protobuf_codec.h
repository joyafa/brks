
#ifndef BRKS_INTF_PROTOBUF_PROTOCOL_CODEC_H_
#define BRKS_INTF_PROTOBUF_PROTOCOL_CODEC_H_

#include "glo_def.h"
#include "protocol_head.h"
#include "protocol_codec.h"

class protobuf_protocol_codec_t : public protocol_codec_t
{
public:
	virtual bool encode(iEvent* ev, u8* buffer, u32 size);
	virtual iEvent* decode(u16 mid, u8* buffer, u32 size);
private:
   MobileCodeReqEv* decode_2_mobile_code_req_ev(const u8* buffer, u32 size);
    LoginReqEv* decode_2_login_req_ev(const  u8* buffer, u32 size);
    RechargeReqEv* decode_2_recharge_req_ev(const  u8* buffer, u32 size);
    GetAccountBalanceReqEv* decode_2_get_account_balance_req_ev(const  u8* buffer, u32 size);
    UnlockReqEv* decode_2_unlock_req_ev(const  u8* buffer, u32 size);
    LockReqEv* decode_2_lock_req_ev(const u8* buffer, u32 size);
    ListAccountRecordsReqEv* decode_2_list_account_record_req_ev(const u8* buffer, u32 size);
    ListTravelRecordsReqEv* decode_2_list_travel_records_req_ev(const u8* buffer, u32 size);

private:
    virtual bool encode_common_rsp_ev(CommonRspEv* rsp, u8* buffer, u32 size);
    virtual bool encode_get_account_balance_rsp_ev(GetAccountBalanceRspEv* rsp, u8* buffer, u32 size);
    virtual bool encode_list_account_records_rsp_ev(ListAccountRecordsRspEv* rsp, u8* buffer, u32 size);
    virtual bool encode_list_travels_records_rsp_ev(ListTravelRecordsReqEv* rsp, u8* buffer, u32 size);    
    virtual boll (ListAccountRecordsReqEv* rsp, u8* buffer, u32 size);    
};

#endif

