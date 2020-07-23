#include "ctpmkt.h"

ctpmkt::ctpmkt(CThostFtdcMdApi* MDApi, CThostFtdcReqUserLoginField* pUserLogin):
	_MDApi(MDApi),_pUserLogin(pUserLogin)
{
	nRequestID = 1;
}

ctpmkt::~ctpmkt()
{
	if (_MDApi)
	{
		_MDApi->RegisterSpi(nullptr);
		_MDApi->Release();
		_MDApi = nullptr;
	}
}

void ctpmkt::OnFrontConnected()
{	
	if (_pUserLogin)
	{
		_MDApi->ReqUserLogin(_pUserLogin, nRequestID++);
	}
}

void ctpmkt::OnFrontDisconnected(int nReason)
{
	std::cout << "ctpmkt OnFrontDisconnected, Reason:" << nReason << std::endl;
}

void ctpmkt::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "ctpmkt OnRspUserLogin" << std::endl;
	}
	else
	{
		std::cout << "ctpmkt OnRspUserLogin Error£º"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctpmkt::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout<<"ctpmkt OnRspError:"
		<< pRspInfo->ErrorID
		<< pRspInfo->ErrorMsg
		<< std::endl;
}

void ctpmkt::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "Subscribe:" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cout << "OnRspSubMarketData:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}

void ctpmkt::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	std::cout << pDepthMarketData->ActionDay << "\t"
		<< pDepthMarketData->UpdateTime << "."
		<< pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->ExchangeID << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->OpenPrice << ","
		<< pDepthMarketData->HighestPrice << ","
		<< pDepthMarketData->LowestPrice << ","
		<< pDepthMarketData->ClosePrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->UpperLimitPrice << ","
		<< pDepthMarketData->LowerLimitPrice << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 
		<< std::endl;
}
