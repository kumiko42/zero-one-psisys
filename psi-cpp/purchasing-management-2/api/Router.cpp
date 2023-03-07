﻿#include "stdafx.h"
#include "Router.h"
#include "api/Aspect.h"
#include "domain/vo/JsonVO.h"

#ifdef HTTP_SERVER_DEMO
#include "sample/SampleController.h"
#include "user/DepartController.h"
#include "uselib/ws/TestWs.h"
#endif

#define ANDREW
#include "statis/StatisController.h"
#include "prepayment/PrePayController.h"
#include "PurOrder/PurOrderController.h"

#define ZUICHU
#include "payment/PaymentController.h"

#define J4NNA
#include "PurCom/PurComController.h"

#include "finPaymenReq/FinPaymentReqControlle.h"
#include "payment/PaymentController.h"

Router::Router(http_server* sever)
{
	this->server = sever;
}

void Router::initRouter()
{
	//设置静态文件目录
	server->set_public_root_directory("public");
	server->set_static_dir("static/file");

#ifdef HTTP_SERVER_DEMO
	//绑定首页页面
	BIND_GET_ROUTER(server, "/", [](request& req, response& res) {
		res.render_raw_view("./public/test.html");
		}, nullptr);

	//初始化一个文件上传接口示例
	BIND_POST_ROUTER(server, "/upload-file", [](request& req, response& res) {
		if (req.get_content_type() != content_type::multipart)
		{
			JsonVO vo = JsonVO("", RS_CONTENT_TYPE_ERR);
			nlohmann::json jvo = nlohmann::json(vo);
			jvo.erase("data");
			res.render_json(jvo);
			return;
		}
	//获取表单参数
	std::cout << "nickname:" << req.get_multipart_value_by_key1("nickname") << std::endl;
	std::cout << "age:" << req.get_multipart_value_by_key1("age") << std::endl;
	//获取文件路径
	auto& files = req.get_upload_files();
	std::vector<string> filePaths;
	for (auto& file : files) {
		filePaths.push_back(file.get_file_path().substr(1));
		std::cout << "path " << file.get_file_path() << ",size " << file.get_file_size() << std::endl;
	}
	res.render_json(nlohmann::json(JsonVO<std::vector<std::string>>(filePaths, RS_SUCCESS)));
		}, nullptr);

	createSampleRouter();
	createUserDepartRouter();
	TestWs::addChatHandler(server);
#endif

	//#TIP :系统扩展路由定义，写在这个后面
	createStatisRouter();
	createPaymentRouter();
	createPrePayRouter();
	createPurComRouter();
	createPurOrderRouter();
}

#ifdef HTTP_SERVER_DEMO
void Router::createSampleRouter()
{
	BIND_GET_ROUTER(server, "/get", &SampleController::querySample, nullptr);
	BIND_POST_ROUTER(server, "/post", &SampleController::addSample, nullptr);
	BIND_PUT_ROUTER(server, "/put", &SampleController::modifySample, nullptr);
	BIND_DEL_ROUTER(server, "/delete", &SampleController::removeSample, nullptr);
	BIND_DEL_ROUTER(server, "/delete-by-id", &SampleController::removeById, nullptr);
	BIND_POST_ROUTER(server, "/json", &SampleController::jsonSample, nullptr);
	BIND_POST_ROUTER(server, "/modify-user-info", &SampleController::modifyUserInfo, nullptr);
}

void Router::createUserDepartRouter()
{
	BIND_POST_ROUTER(server, "/depart-add", &DepartController::addDepart, nullptr);
	BIND_POST_ROUTER(server, "/depart-add-more", &DepartController::addDepartMore, nullptr);
}
#endif

/**
* Router绑定接口函数实现
*/
void Router::createStatisRouter() {
	// 采购统计Router
	// 查询
	BIND_GET_ROUTER(server, "/query/ExecuteStatus", &StatisController::queryExeStatus, nullptr);
	BIND_GET_ROUTER(server, "/query-all", &StatisController::queryStatis, nullptr);
	BIND_GET_ROUTER(server, "/query-byDept", &StatisController::queryStatisByDept, nullptr);
	BIND_GET_ROUTER(server, "/query-byOperator", &StatisController::queryStatisByOperator, nullptr);
	BIND_GET_ROUTER(server, "/query-bySupplier", &StatisController::queryStatisBySupplier, nullptr);
	BIND_GET_ROUTER(server, "/query-byMaterial", &StatisController::queryStatisByMaterial, nullptr);
	// 导出
	BIND_GET_ROUTER(server, "/export/ExecuteStatus", &StatisController::exportExeStatus, nullptr);
	BIND_GET_ROUTER(server, "/export-all", &StatisController::exportStatis, nullptr);
	BIND_GET_ROUTER(server, "/export-byDept", &StatisController::exportStatisByDept, nullptr);
	BIND_GET_ROUTER(server, "/export-byOperator", &StatisController::exportStatisByOperator, nullptr);
	BIND_GET_ROUTER(server, "/export-bySupplier", &StatisController::exportStatisBySupplier, nullptr);
	BIND_GET_ROUTER(server, "/export-byMaterial", &StatisController::exportStatisByMaterial, nullptr);
}

// 采购付款申请
// 负责人：最初
void Router::createPaymentRouter()
{
	//修改单据状态
	BIND_PUT_ROUTER(server, "/modify-payment-status", &PaymentController::modifyPayment, nullptr);
	//删除单据
	BIND_DEL_ROUTER(server, "/DePayment", &PaymentController::deleteById, nullptr);
	//添加单据
	BIND_PUT_ROUTER(server, "/AppPayment", &PaymentController::AddPayment, nullptr);

	//查询列表
	BIND_GET_ROUTER(server, "/finPaymenReq", &FinPaymentReqControlle::queryFinPaymentReq, nullptr);
	//查询单个详情
	BIND_GET_ROUTER(server, "/finPaymentReqEntry", &FinPaymentReqControlle::queryFinPaymentReqEntry, nullptr);
	BIND_POST_ROUTER(server, "/paymentReq/add", &FinPaymentReqControlle::addPaymentReq, nullptr);
	//修改订单
	BIND_PUT_ROUTER(server, "/paymentReq/mod", &FinPaymentReqControlle::modPaymentReq, nullptr);
}

void Router::createPrePayRouter()
{
	//查询单据列表
	// 负责人：Qi
	BIND_GET_ROUTER(server, "/get-bill-list", &PrePayController::queryPayFindBill1, nullptr);
	//查询指定单据详细信息
	// 负责人：Qi
	BIND_GET_ROUTER(server, "/get-detail-bill", &PrePayController::queryPayFinDetailBill1, nullptr);
	//添加申请(保存/提交)
	// 负责人：Qi
	BIND_POST_ROUTER(server, "/post-add", &PrePayController::addPay, nullptr);
	//修改申请(保存/提交/审核)
	// 负责人：Qi
	BIND_PUT_ROUTER(server, "/put-modifyPay", &PrePayController::modifyPay, nullptr);

	// 采购预付申请-修改单据状态(关闭/作废/反关闭)
	// 负责人：Andrew
	BIND_PUT_ROUTER(server, "/modify-bill-status", &PrePayController::modifyPurReqBillStatus, nullptr);

	//xubuxi
	//采购订单列表
	BIND_GET_ROUTER(server, "/get-prepaybill", &PrePayController::queryPayFindBill2, nullptr);
	//指定订单详细信息
	BIND_GET_ROUTER(server, "/get-prepaybill-detail", &PrePayController::queryPayFinDetailBill2, nullptr);
	//删除申请
	BIND_DEL_ROUTER(server, "/delete-t-prepay-id", &PrePayController::removeDePayId, nullptr);
}

// 比价单列表查询及明细列表查询
// 负责人：J4nnA
void Router::createPurComRouter() {
	// 分页数据
	// 负责人：J4nnA
	BIND_GET_ROUTER(server, "/purCom/list", &PurComController::listPurCom, nullptr);
	// 明细列表
	// 负责人：J4nnA
	BIND_GET_ROUTER(server, "/purCom/listEntrys", &PurComController::listPurComEntrys, nullptr);
}

// 采购订单
// 负责人：青羽, Andrew
void Router::createPurOrderRouter()
{
	// 分页数据
	// 负责人：青羽
	BIND_GET_ROUTER(server, "/purOrder/list", &PurOrderController::querylistPurOrder, nullptr);
	// 单个数据
	// 负责人：青羽
	BIND_GET_ROUTER(server, "/purOrder/queryEntryByMainId", &PurOrderController::getPurOrder, nullptr);
	// 新增数据
	// 负责人：青羽
	BIND_POST_ROUTER(server, "/purOrder/add", &PurOrderController::addPurOrder, nullptr);
	// 修改数据
	// 负责人：青羽
	BIND_PUT_ROUTER(server, "/purOrder/edit", &PurOrderController::modifyPurOrder, nullptr);

	// 采购订单删除
	// 负责人：Andrew
	BIND_DEL_ROUTER(server, "/purOrder/deleteById", &PurOrderController::removeById, nullptr);
	// 采购订单-修改状态
	// 负责人：Andrew
	BIND_PUT_ROUTER(server, "/purOrder/modifyStatus", &PurOrderController::statusPurOrder, nullptr);
}
