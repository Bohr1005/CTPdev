#pragma once
#include "ctptrd.h"
#include<thread>

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
		std::cout << "ctptrd OnRspUserLogin Error："
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctptrd::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (nullptr != pSettlementInfoConfirm)
		{
			std::cout << "OnRspQrySettlementInfoConfirm:"
				<< pSettlementInfoConfirm->ConfirmDate << ","
				<< pSettlementInfoConfirm->ConfirmTime << ","
				<< bIsLast
				<< std::endl;

			std::string lastConfirmDate = pSettlementInfoConfirm->ConfirmDate;

			if (lastConfirmDate != tradingDate) {
				//今天还没确认过, 第一次发送交易指令前，查询投资者结算结果
				std::this_thread::sleep_for(std::chrono::seconds(1));

				CThostFtdcQrySettlementInfoField field;
				memset(&field, 0, sizeof(CThostFtdcQrySettlementInfoField));
				strcpy(field.BrokerID, pSettlementInfoConfirm->BrokerID);
				strcpy(field.InvestorID, pSettlementInfoConfirm->InvestorID);
				strcpy(field.TradingDay, pSettlementInfoConfirm->ConfirmDate);

				_trdApi->ReqQrySettlementInfo(&field, nRequestID++);
			}
			else {
				//今天已经确认
				CThostFtdcQryTradingAccountField account;
				memset(&account, 0, sizeof(CThostFtdcQryTradingAccountField));
				strcpy(account.BrokerID, pSettlementInfoConfirm->BrokerID);
				strcpy(account.InvestorID, pSettlementInfoConfirm->InvestorID);
				_trdApi->ReqQryTradingAccount(&account, nRequestID++);
			}
		}
	}
	else {
		std::cout << "ctptrd OnRspQrySettlementInfoConfirm Error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctptrd::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "ctptrd OnRspQrySettlementInfo"<< std::endl;
	std::cout << pSettlementInfo->Content << std::endl;
	if (pRspInfo == nullptr || pRspInfo->ErrorID == 0) {
		if (bIsLast == true) {
			//确认投资者结算结果
			CThostFtdcSettlementInfoConfirmField field;
			memset(&field, 0, sizeof(CThostFtdcQryTradingAccountField));
			strcpy(field.BrokerID, pSettlementInfo->BrokerID);
			strcpy(field.InvestorID, pSettlementInfo->InvestorID);
			_trdApi->ReqSettlementInfoConfirm(&field, nRequestID++);
		}
	}
	else {
		std::cout << "OnRspQrySettlementInfo Error:" 
			<< pRspInfo->ErrorID 
			<< pRspInfo->ErrorMsg 
			<< std::endl;
	}
}

void ctptrd::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (pSettlementInfoConfirm != nullptr) {
			std::cout << "BrokerID:" << pSettlementInfoConfirm->BrokerID << std::endl
				<< "InvestorID:" << pSettlementInfoConfirm->InvestorID << std::endl
				<< "ConfirmDate：" << pSettlementInfoConfirm->ConfirmDate << std::endl
				<< "ConfirmTime：" << pSettlementInfoConfirm->ConfirmTime << std::endl;

			//查询所有可用合约信息
			CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField{ 0 };
			_trdApi->ReqQryInstrument(&myreq, nRequestID++);
		}
		else {
			std::cout << "SettlementInfoConfirm:" << pSettlementInfoConfirm << std::endl;
		}
	}
	else {
		std::cout << "OnRspUserLogin error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctptrd::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast)
		{
			std::cout << "All instruments have been  queried " << std::endl;
		}
		else {
			CThostFtdcInstrumentField instrument;
			std::cout << "query:" << pInstrument->InstrumentID << std::endl;
			memcpy(&instrument, pInstrument, sizeof(CThostFtdcInstrumentField));
			instruments.push_back(instrument);
		}
	}
	else {
		std::cout << "OnRspQryInstrument error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}



