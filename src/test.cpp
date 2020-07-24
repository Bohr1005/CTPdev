#include "ctptrd.h"
#include "ctpmkt.h"
#include <cstdio>
#include "document.h"
#include "filereadstream.h"

using namespace rapidjson;
int main()
{
	Document d;
	std::FILE* fp = fopen("F:\\CTPdev\\src\\config.json", "rb");
	char buffer[50];
	FileReadStream frs(fp, buffer, sizeof(buffer));
	d.ParseStream(frs);
	auto userid = d["userid"].GetString();
	auto passwd = d["passwd"].GetString();
	auto brokerid = d["brokerid"].GetString();
	auto mdfront = d["mktfront"].GetString();
	auto trdfront = d["trdfront"].GetString();
	fclose(fp);
	std::cout << "userid:"<< userid << std::endl;
	std::cout << "passwd:" << passwd << std::endl;
	std::cout << "brokerid:" << brokerid << std::endl;
	std::cout << "mdfront:" << mdfront << std::endl;
	std::cout << "trdfront:" << trdfront << std::endl;
	CThostFtdcMdApi* MDApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcReqUserLoginField *pLoginField = new CThostFtdcReqUserLoginField();
	strcpy(pLoginField->UserID, userid);
	strcpy(pLoginField->Password, passwd);
	strcpy(pLoginField->BrokerID, brokerid);
	//CThostFtdcTraderApi* trdApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	ctpmkt test_mkt(MDApi,pLoginField);
	//ctptrd *test_trd = new ctptrd(trdApi, pLoginField);
	MDApi->RegisterSpi(&test_mkt);
	MDApi->RegisterFront((char*)mdfront);
	//trdApi->RegisterFront((char*)trdfront);
	//trdApi->RegisterSpi(test_trd);
	MDApi->Init();
	MDApi->Join();
	//trdApi->Init();
};