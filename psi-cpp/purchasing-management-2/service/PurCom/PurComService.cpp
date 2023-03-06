/*
 Copyright Zero One Star. All rights reserved.

 @Author: qingyu
 @Date: 2023/02/17 11:13:11

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "PurComService.h"
#include "../../dao/PurCom/PurComDAO.h"

// ��ҳ��ѯ��������
PageVO<PurComVO> PurComService::listAll(const PurComQuery& query)
{
	// �������ض���
	PageVO<PurComVO> pages;
	pages.setPageIndex(query.getPageIndex());
	pages.setPageSize(query.getPageSize());

	// ����DO
	PurComDO obj;
	obj.setBill_no(query.getBill_no());
	obj.setBill_date(query.getBill_date());
	
	// ����DAO�����
	PurComDAO dao;
	
	//��ѯ����������
	uint64_t count = dao.count(obj);
	//
	//������ɸѡ����
	//
	if (count <= 0)
	{
		return pages;
	}

	// ����DAO�㹦��,����DO
	pages.setTotal(count);
	pages.calcPages();
	list<PurComDO> result = dao.selectPurCom(obj, query.getPageIndex(), query.getPageSize());
	list<PurComVO> vr;		//��ҳ��ѯ���ص�VO�б�
	// ����DAO�㷵�ص�DO����VO��Ա���и�ֵ

	for (PurComDO sub : result)
	{
		// �������ڷ��ص�VO
		PurComVO vo;

		vo.setId(sub.getId());
		vo.setBill_no(sub.getBill_no());
		vo.setBill_date(sub.getBill_date());

		vo.setSrc_bill_type(sub.getSrc_bill_type());
		vo.setSrc_bill_id(sub.getSrc_bill_id());
		vo.setSrc_no(sub.getSrc_no());

		vo.setSubject(sub.getSubject());
		vo.setIs_rubric(sub.getIs_rubric());
		vo.setCandidate_quot_ids(sub.getCandi_date_quot_ids());
		
		vo.setPayment_method(sub.getPayment_method());
		vo.setDelivery_place(sub.getDelivery_place());
		vo.setDelivery_time(sub.getDelivery_time());

		vo.setAttachment(sub.getAttachment());
		vo.setRemark(sub.getRemark());
		vo.setIs_auto(sub.getIs_auto());

		vo.setBill_stage(sub.getBill_stage());
		vo.setApprover(sub.getApprover());
		vo.setBpmi_instance_id(sub.getBpmi_instance_id());

		vo.setApproval_result_type(sub.getApproval_result_type());
		vo.setApproval_remark(sub.getApproval_remark());
		vo.setIs_effective(sub.getIs_effective());
		
		vo.setEffective_time(sub.getEffective_time());
		vo.setIs_closed(sub.getIs_closed());
		vo.setIs_voided(sub.getIs_voided());

		vo.setSys_org_code(sub.getSys_org_code());
		vo.setCreate_by(sub.getCreate_by());
		vo.setCreate_time(sub.getCreate_time());
		
		vo.setUpdate_by(sub.getUpdate_by());
		vo.setUpdate_time(sub.getUpdate_time());
		vo.setVersion(sub.getVersion());

		vr.push_back(vo);
	}
	// ��vr����pages�У�������pages
	pages.setRows(vr);
	return pages;
}

// ��ѯ��������
PurComVO PurComService::getData(uint64_t id) {
	PurComVO data;

	return data;
}

// ��ѯָ���ȼ۵�����ϸ�б�
PageVO<PurComEntryVO> PurComService::listEntry(const PurComEntryQuery& query)
{
	// ���췵�ض���
	PageVO<PurComEntryVO> pages;
	pages.setPageIndex(query.getPageIndex());
	pages.setPageSize(query.getPageSize());

	// ����DO
	PurComEntryDO obj;
	obj.setMid(query.getId());

	// ����DAO�����
	PurComDAO dao;
	
	// ��ѯ����������
	uint64_t count = dao.countEntrys(obj);
	//
	// ������ɸѡ����
	//
	if (count <= 0)
	{
		return pages;
	}
	
	// ����DAO�㹦��,����DO
	pages.setTotal(count);
	pages.calcPages();
	list<PurComEntryDO> result = dao.selectPurComEntry(obj, query.getPageIndex(), query.getPageSize());
	list<PurComEntryVO> vr;		//��ϸ�б��ѯ���ص�VO�б�

	// ����DAO�㷵�ص�DO����VO��Ա���и�ֵ
	for (PurComEntryDO sub : result)
	{
		// �������ڷ��ص�VO
		PurComEntryVO vo;

		vo.setId(sub.getId());
		vo.setMid(sub.getMid());
		vo.setBill_no(sub.getBill_no());
		vo.setEntry_no(sub.getEntry_no());
		vo.setSrc_bill_type(sub.getSrc_bill_type());
		vo.setSrc_bill_id(sub.getSrc_bill_id());

		vo.setSrc_entry_id(sub.getSrc_entry_id());
		vo.setSrc_no(sub.getSrc_no());
		vo.setSupplier_id(sub.getSupplier_id());

		vo.setMaterial_id(sub.getMaterial_id());
		vo.setUnit_id(sub.getUnit_id());
		vo.setQty(sub.getQty());

		vo.setTax_rate(sub.getTax_rate());
		vo.setPrice(sub.getPrice());
		vo.setDiscount_rate(sub.getDiscount_rate());

		vo.setAmt(sub.getAmt());
		vo.setRanking(sub.getRanking());
		vo.setRemark(sub.getRemark());
		vo.setCustom1(sub.getCustom1());

		vo.setCustom2(sub.getCustom2());
		vo.setVersion(sub.getVersion());

		vr.push_back(vo);
	}
	// ��vr����pages�У�������pages
	pages.setRows(vr);
	return pages;
}
