#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: qingyu
 @Date: 2023/02/25 12:02:32

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
#ifndef _PUR_COM_DO_H_
#define _PUR_COM_DO_H_
#include "../DoInclude.h"

//��DO�޸Ĺ����޸��ˣ�J4nnA

/**
 * �ɹ��ȼ���
 */
class PurComDO
{
	// �����޸���ID����ΪString
	CC_SYNTHESIZE(string, id, Id);
	// ���ݱ��
	CC_SYNTHESIZE(string, bill_no, Bill_no);
	// ��������_��ʼ����
	CC_SYNTHESIZE(string, bill_date, Bill_date);
	// ��������_��������___J4nnA׷���˸��ֶ�
	CC_SYNTHESIZE(string, bill_date_end, Bill_date_end);
	// Դ������
	CC_SYNTHESIZE(string, src_bill_type, Src_bill_type);
	// Դ��id
	CC_SYNTHESIZE(string, src_bill_id, Src_bill_id);
	// Դ����
	CC_SYNTHESIZE(string, src_no, Src_no);
	// ��������
	CC_SYNTHESIZE(string, subject, Subject);
	// ���ֵ���
	CC_SYNTHESIZE(int, is_rubric, Is_rubric);
	// ��ѡ���۵�ids
	CC_SYNTHESIZE(string, candidate_quot_ids, Candi_date_quot_ids);
	// ���ʽ
	CC_SYNTHESIZE(string, payment_method, Payment_method);
	// �����ص�
	CC_SYNTHESIZE(string, delivery_place, Delivery_place);
	// ��������
	CC_SYNTHESIZE(string, delivery_time, Delivery_time);
	// ����
	CC_SYNTHESIZE(string, attachment, Attachment);
	// ��ע
	CC_SYNTHESIZE(string, remark, Remark);
	// �Ƿ��Զ�����
	CC_SYNTHESIZE(int, is_auto, Is_auto);
	// ���ݽ׶�
	CC_SYNTHESIZE(string, bill_stage, Bill_stage);
	// ������
	CC_SYNTHESIZE(string, approver, Approver);
	// ���ʵ��id
	CC_SYNTHESIZE(string, bpmi_instance_id, Bpmi_instance_id);
	// �����������
	CC_SYNTHESIZE(string, approval_result_type, Approval_result_type);
	// ���
	CC_SYNTHESIZE(string, approval_remark, Approval_remark);
	// �Ƿ���Ч
	CC_SYNTHESIZE(int, is_effective, Is_effective);
	// ��Чʱ��
	CC_SYNTHESIZE(string, effective_time, Effective_time);
	// �Ƿ�ر�
	CC_SYNTHESIZE(int, is_closed, Is_closed);
	// �Ƿ�����
	CC_SYNTHESIZE(int, is_voided, Is_voided);
	// ��������
	CC_SYNTHESIZE(string, sys_org_code, Sys_org_code);
	// ������
	CC_SYNTHESIZE(string, create_by, Create_by);
	// ����ʱ��
	CC_SYNTHESIZE(string, create_time, Create_time);
	// �޸���
	CC_SYNTHESIZE(string, update_by, Update_by);
	// �޸�ʱ��
	CC_SYNTHESIZE(string, update_time, Update_time);
	// �汾
	CC_SYNTHESIZE(int, version, Version);
public:
	PurComDO() {

	}
};
#endif // !_PUR_COM_DO_H_ 