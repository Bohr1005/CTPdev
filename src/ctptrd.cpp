#pragma once
#include "ctptrd.h"

ctptrd::ctptrd(CThostFtdcTraderApi* trdApi, CThostFtdcReqUserLoginField* pUserLogin):
	_trdApi(trdApi),_pUserLogin(pUserLogin)
{
	nRequestID = 1;
}

void ctptrd::OnFrontConnected()
{
	if (_pUserLogin)
	{
		_trdApi->ReqUserLogin(_pUserLogin, nRequestID++);
	}
}

void ctptrd::OnFrontDisconnected(int nReason)
{
	std::cout << "ctptrd OnFrontDisconnected, Reason:" << nReason << std::endl;
}

void ctptrd::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "ctptrd OnRspUserLogin" << std::endl;
		this->tradingDate = _trdApi->GetTradingDay();
		std::cout << "Trading day:" << this->tradingDate.c_str() << std::endl;
		std::cout << "ctptrd login successfully" << std::endl;
		std::cout << "FrontID:" << pRspUserLogin->FrontID << std::endl;
		std::cout << "SessionID:" << pRspUserLogin->SessionID << std::endl;
		std::cout << "MaxOrderRef:" << pRspUserLogin->MaxOrderRef << std::endl;
		std::cout << "SHFE TIME:" << pRspUserLogin->SHFETime << std::endl;
		std::cout << "DCE TIME:" << pRspUserLogin->DCETime << std::endl;
		std::cout << "CZCE TIME:" << pRspUserLogin->CZCETime << std::endl;
		std::cout << "FFEX TIME:" << pRspUserLogin->FFEXTime << std::endl;
		std::cout << "INE TIME:" << pRspUserLogin->INETime << std::endl;

		CThostFtdcQrySettlementInfoConfirmField pQrySettlementInfoConfirm;
		memset(&pQrySettlementInfoConfirm, 0, sizeof(CThostFtdcQrySettlementInfoConfirmField));
		strcpy(pQrySettlementInfoConfirm.BrokerID, pRspUserLogin->BrokerID);
		strcpy(pQrySettlementInfoConfirm.InvestorID, pRspUserLogin->UserID);
		_trdApi->ReqQrySettlementInfoConfirm(&pQrySettlementInfoConfirm, nRequestID++);
	}
	else
	{
		std::cout << "ctptrd OnRspUserLogin Error£º"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctptrd::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

}

void ctptrd::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

}

void ctptrd::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

}


