#include "stdafx.h"
#include "CgthckService.h"

list<QueryCgrkBillListsVO> CgthckService::listAll(const QueryCgrkBillQuery& query)
{
    // ��װ��ѯ����
    CgthckEntryDO obj;
    // ���ݱ��
    obj.setBillNo(query.getBillNo());

    CgthckDAO dao;

    list<CgthckEntryDO> result = dao.selectWithId(obj);
    list< QueryCgrkBillListsVO> vr;
    for (auto sub : result)
    {
        QueryCgrkBillListsVO vo;
        vo.setEntryNo(sub.getEntryNo());
        vo.setSrcNo(sub.getSrcNo());
        vo.setWarehouseId(sub.getWarehouseId());
        vo.setBatchNo(sub.getBatchNo());
        vo.setUnitId(sub.getUnitId());
        vo.setSettleQty(sub.getSettleQty());
        vo.setTaxRate(sub.getTaxRate());
        vo.setPrice(sub.getPrice());
        vo.setDiscountRate(sub.getDiscountRate());
        vo.setTax(sub.getTax());
        vo.setSettleAmt(sub.getSettleAmt());
        vo.setQty(sub.getQty());
        vo.setCost(sub.getCost());
        vo.setInvoicedQty(sub.getInvoicedQty());
        vo.setInvoicedAmt(sub.getInvoicedAmt());
        vo.setRemark(sub.getRemark());
        vo.setCustom1(sub.getCustom1());
        vo.setCustom2(sub.getCustom2());
        vr.push_back(vo);
    }
    return vr;
}

int CgthckService::saveData(const AddCgthckBillDTO& dto)
{
    // �������ݼ���ĺ���
    auto check = [](const AddCgthckBillDTO& dto)
    {
        for (auto& ety : dto.getEntry())
        {
            // ��¼�Ų���Ϊ�������Һ��ֵ������ݲ���Ϊ����
            if (ety.getEntryNo() < 0 || ety.getSettleQty() > 0 || ety.getTax() > 0 || ety.getSettleAmt() > 0)
                return true;
        }
        return false;
    };

    // ����У��
    if (check(dto))
        return -1;

    // ����dao�����
    CgthckDAO dao;

    // �ϴ�����
    string attachment{ "" };
    for (auto& f : dto.getFiles())
    {
        string fileName = dao.insertFile(f);
        if (!fileName.empty())
        {
            attachment += fileName + ',';
        }
    }


    // ������װ���κź���
    auto getBanchNo = [](string billNO, string entryNo) -> std::string
    {
        string banchNo = billNO + '-' + entryNo;
        return banchNo;
    };

    // ����ѩ��ID
    SnowFlake sf(1, 5);
    string id = to_string(sf.nextId());

    // ��װ��������
    CgthckDO data;
    // id
    data.setId(id);
    // ���ݱ��
    data.setBillNo(dto.getBillNo());
    // ��������
    data.setBillDate(dto.getBillDate());
    // ��Ӧ��
    data.setSupplierId(dto.getSupplierId());
    // Դ������
    data.setSrcBillType(dto.getSrcBillType());
    // �ɹ���ⵥ���
    data.setSrcNo(dto.getSrcNo());
    // ����Ч
    data.setIsEffective(dto.getIsEff());
    // �ѹر�
    data.setIsClosed(dto.getIsClosed());
    // ������
    data.setIsVoided(dto.getIsVoided());
    // ҵ����
    data.setOpDept(dto.getOpDept());
    // ����
    data.setSubject(dto.getSubject());
    // ��Ʊ����
    data.setInvoiceType(dto.getInvoiceType());
    // ���⾭��
    data.setHandler(dto.getHandler());

    // ��������
    dao.getSqlSession()->beginTransaction();

    // ִ���������
     int cnt = 0; // ��¼������ϸ��������
    // ��ȡmid
    uint64_t mid = dao.insert(data);
    if (mid > 0)
    {
        // ��װ��ϸ����
        list<CgthckBillEntryDTO> entries = dto.getEntry();
        for (auto& ety : entries)
        {
            CgthckEntryDO entryData;
            // id
            entryData.setId(to_string(sf.nextId()));
            // ����id
            entryData.setMid(to_string(mid));
            // ���ݱ��
            entryData.setBillNo(dto.getBillNo());
            // ��¼��
            entryData.setEntryNo(to_string(ety.getEntryNo()));
            // ����
            entryData.setMaterialId(ety.getMaterialId());
            // ���κ�
            entryData.setBatchNo(getBanchNo(dto.getBillNo(), to_string(ety.getEntryNo())));
            // �ֿ�
            entryData.setWarehouseId(ety.getWarehouseId());
            // ���뷽��
            entryData.setStockIoDirection("1"); // ��ʱ��֪���ĸ�����������
            // ������λ
            entryData.setUnitId(ety.getUnitId());
            // ��������
            entryData.setSettleQty(ety.getSettleQty());
            // ˰��
            entryData.setTaxRate(ety.getTaxRate());
            // ��˰����
            entryData.setPrice(ety.getPrice());
            // �ۿ���
            entryData.setDiscountRate(ety.getDiscountRate());
            // ˰��
            entryData.setTax(ety.getTax());
            // ������
            entryData.setSettleAmt(ety.getSettleAmt());
            // �������
            entryData.setQty(ety.getQty());
            // �ɹ�����(�ɱ�)
            entryData.setCost(ety.getCost());
            // �����
            entryData.setInvoicedAmt(ety.getInAmt());

            // ִ���������, ���ز���ɹ�������
            cnt += dao.insert(entryData);
        }
    }
    if (mid <= 0 || cnt <= 0)
    {
        // ����ʧ��, �ع�
        dao.getSqlSession()->rollbackTransaction();
    }
    else
    {
        // ����ɹ�, �ύ
        dao.getSqlSession()->commitTransaction();
    }
    return cnt;
}

int CgthckService::updateData(const AddCgthckBillDTO& dto)
{
    // ���ݼ���
    auto check = [](const AddCgthckBillDTO& dto)
    {
        for (auto& ety : dto.getEntry())
        {
            // ��¼�Ų���Ϊ�������Һ��ֵ������ݲ���Ϊ����
            if (ety.getEntryNo() < 0 || ety.getSettleQty() > 0 || ety.getTax() > 0 || ety.getSettleAmt() > 0)
                return true;
        }
        return false;
    };

    // ������ݴ���"����Ч"�׶� �����޸�
    if (dto.getIsClosed() || check(dto))
    {
        return -1;
    }
    
    // ����dao�����
    CgthckDAO dao;

    // �ϴ�����
    string attachment{ "" };
    for (auto& f : dto.getFiles())
    {
        string fileName = dao.insertFile(f);
        if (!fileName.empty())
        {
            attachment += fileName + ',';
        }
    }

    // ������װ���κź���
    auto getBanchNo = [](string billNO, string entryNo) -> std::string
    {
        string banchNo = billNO + '-' + entryNo;
        return banchNo;
    };

    // ��װ��������
    CgthckDO data;
    // ���ݱ��
    data.setBillNo(dto.getBillNo());
    // ��������
    data.setBillDate(dto.getBillDate());
    // ��Ӧ��
    data.setSupplierId(dto.getSupplierId());
    // �ɹ���ⵥ���
    data.setSrcNo(dto.getSrcNo());
    // ����Ч
    data.setIsEffective(dto.getIsEff());
    // �ѹر�
    data.setIsClosed(dto.getIsClosed());
    // ������
    data.setIsVoided(dto.getIsVoided());
    // ҵ����
    data.setOpDept(dto.getOpDept());
    // ����
    data.setSubject(dto.getSubject());
    // ��Ʊ����
    data.setInvoiceType(dto.getInvoiceType());
    // ���⾭��
    data.setHandler(dto.getHandler());
    // ���ݽ׶�
    data.setBillStage(dto.getBillStage());
    // ����
    data.setAttachment(attachment);
    // ��ע
    data.setRemark(dto.getRemark());

    // ��������
    dao.getSqlSession()->beginTransaction();

    // ִ���������
    int row = dao.update(data);
    if (row == 0)
    {
        dao.getSqlSession()->rollbackTransaction();
        // ɾ�������ĸ���
        dao.deleteFile(attachment);
        return -2;
    }

    // ��װ��ϸ����
    CgthckEntryDO entryData;
    list<CgthckBillEntryDTO> entries = dto.getEntry();
    for (auto& ety : entries)
    {
        // ���α��
        entryData.setBatchNo(getBanchNo(dto.getBillNo(), to_string(ety.getEntryNo())));
        // ��¼��
        entryData.setEntryNo(to_string(ety.getEntryNo()));
        // ��������
        entryData.setSettleQty(ety.getSettleQty());
        //˰��
        entryData.setTaxRate(ety.getTaxRate());
        // ������
        entryData.setSettleAmt(ety.getSettleAmt());
        // �ɹ�����
        entryData.setCost(ety.getCost());
        // �������
        entryData.setQty(ety.getQty());
        // ��ע
        entryData.setRemark(ety.getRemark());
        // �Զ���1
        entryData.setCustom1(ety.getCustom1());
        // �Զ���2
        entryData.setCustom2(ety.getCustom2());

        if (dao.update(entryData) == 0)
        {
            dao.getSqlSession()->rollbackTransaction();
            return -3;
        }
    }
    // ����ɹ�, �ύ
    dao.getSqlSession()->commitTransaction();
    return row;
}

int CgthckService::updateApproval(const ModifyCgthckBillDTO& dto, const PayloadDTO& payload)
{
    // ���ݼ��� ֻ�д��ڱ�����������
    if (dto.getBillStage().compare("14")) // 12������, 14������ 
    {
        return -1;
    }

    // ��װ����
    CgthckDO data;
    data.setBillNo(dto.getBillNo());
    data.setRemark(dto.getRemark());
    data.setApprovalRemark(dto.getApprovalRemark());
    data.setApprovalResultType(dto.getApprovalResultType());
    data.setApprover(payload.getUsername());

    // ���ɵ�ǰʱ��
    time_t rawtime;
    struct tm* info;
    char buffer[80];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    // ��������
    if (stoi(data.getApprovalResultType()) == 1) // ���ͨ��
    {
        data.setIsEffective(1);
        data.setEffectiveTime(string(buffer));
        data.setIsClosed(1);
        data.setBillStage("34"); // 34:ִ����
    }
    else
    {
        data.setBillStage("24"); // 24:������
    }

    // ����DAO��
    CgthckDAO dao;
    // ����ʼ
    dao.getSqlSession()->beginTransaction();
    int row = dao.updateApproval(data);
    if (row == 0)
    {
        dao.getSqlSession()->rollbackTransaction();
        return -2;
    }
    dao.getSqlSession()->commitTransaction();
    return row;
}

int CgthckService::removeData(const DeleteCgthckBillDTO& dto)
{
    CgthckDAO dao;
    int row = -1;
    
    // ��鵥�ݱ���Ƿ�Ϊ��
    if (!dto.getBillNo().empty())
    {
        // ɾ������
        row = dao.deleteBillById(dto.getBillNo());
        // ɾ����ϸ
        row += dao.deleteEntryById(dto.getBillNo());
    }
    return row;
}

int CgthckService::removeEntry(const DeleteCgthckBillDTO& dto)
{
    CgthckDAO dao;

    int row = -1;
    // �����ϸ�б��Ƿ�Ϊ��
    if (!dto.getBillNo().empty() && !dto.getEntries().empty())
    {
        string billNo = dto.getBillNo();
        for (auto& entry : dto.getEntries())
        {
            // ɾ����ϸ
            row += dao.deleteEntryById(billNo, entry);
        }
    }
    return row;
}

int CgthckService::closed(const ModifyCgthckBillDTO& dto, const PayloadDTO& payload)
{
    // ��װ����
    CgthckDO data;
    data.setBillNo(dto.getBillNo());
    data.setIsClosed(1);

    // ���ɵ�ǰʱ��
    time_t rawtime;
    struct tm* info;
    char buffer[80];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    data.setEffectiveTime(string(buffer));
    data.setApprover(payload.getUsername());

    CgthckDAO dao;
    return dao.updateStatus(data);
}

int CgthckService::unclosed(const ModifyCgthckBillDTO& dto, const PayloadDTO& payload)
{
    // ��װ����
    CgthckDO data;
    data.setBillNo(dto.getBillNo());
    data.setIsClosed(0);

    // ���ɵ�ǰʱ��
    time_t rawtime;
    struct tm* info;
    char buffer[80];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    data.setEffectiveTime(string(buffer));
    data.setApprover(payload.getUsername());

    CgthckDAO dao;
    return dao.updateStatus(data);
}

int CgthckService::voided(const ModifyCgthckBillDTO& dto, const PayloadDTO& payload)
{
    // ��װ����
    CgthckDO data;
    data.setBillNo(dto.getBillNo());
    data.setIsVoided(1);

    // ���ɵ�ǰʱ��
    time_t rawtime;
    struct tm* info;
    char buffer[80];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    data.setEffectiveTime(string(buffer));
    data.setApprover(payload.getUsername());

    CgthckDAO dao;
    return dao.updateStatus(data);
}

int CgthckService::importData(const ImportCgthckFileDTO& dto)
{
    // ����Excel�е�����
    ExcelComponent excel;
    string fileName = dto.getFiles().front();
    string sheetName = CharsetConvertHepler::ansiToUtf8("����ⵥ");
    string entrySheetName = CharsetConvertHepler::ansiToUtf8("��ϸ");
    // ���ļ�������ת���ɶ�ά����, ����:��ά����ĵ�һ����header(����), ����Ҫ�������ݿ�
    vector<vector<string>> data = excel.readIntoVector(fileName, sheetName);
    vector<vector<string>> entryData = excel.readIntoVector(fileName, entrySheetName);

    // ����ά����ת��ΪDOģ��
    list<CgthckDO> cgthckDo;
    if (data.size() > 1)
    {
        for (int i = 1; i < data.size(); i++)
        {
            CgthckDO tmpDo;
            int j = 0;
            auto tmpData = data[i];
            // ���������
            tmpDo.setStockIoType(tmpData[j++]);
            // �Ƿ�������
            tmpDo.setHasRp(stoi(tmpData[j++]));
            // �Ƿ����Ƕ�
            tmpDo.setHasSwell(stoi(tmpData[j++]));
            // ��Ӧ��
            tmpDo.setSupplierId(tmpData[j++]);
            // �ͻ�
            tmpDo.setCustomerId(tmpData[j++]);
            // ��Ʊ����
            tmpDo.setInvoiceType(tmpData[j++]);
            // ҵ����
            tmpDo.setOpDept(tmpData[j++]);
            // ҵ��Ա
            tmpDo.setOperator1(tmpData[j++]);
            // ����⾭��
            tmpDo.setHandler(tmpData[j++]);
            // �ɱ�
            tmpDo.setCost(stod(tmpData[j++]));
            // ������
            tmpDo.setSettleAmt(stod(tmpData[j++]));
            // �ѽ�����
            tmpDo.setSettledAmt(stod(tmpData[j++]));
            // �ѿ�Ʊ���
            tmpDo.setInvoicedAmt(stod(tmpData[j++]));
            // �Ƿ���Ч
            tmpDo.setIsEffective(stod(tmpData[j++]));
            // ����
            tmpDo.setAttachment(tmpData[j++]);
            // Դ��id
            tmpDo.setSrcBillId(tmpData[j++]);
            // ��������
            tmpDo.setSubject(tmpData[j++]);
            // ���ݽ׶�
            tmpDo.setBillStage(tmpData[j++]);
            // Դ����
            tmpDo.setSrcNo(tmpData[j++]);
            // �Ƿ��Զ�����
            tmpDo.setIsAuto(stoi(tmpData[j++]));
            // ��ע
            tmpDo.setRemark(tmpData[j++]);
            // ����ʵ��id
            tmpDo.setBpmiInstanceId(tmpData[j++]);
            // ������
            tmpDo.setIsVoided(stoi(tmpData[j++]));
            // ���ݱ��
            tmpDo.setBillNo(tmpData[j++]);
            // �Ƿ����
            tmpDo.setIsRubric(stoi(tmpData[j++]));
            // Դ������
            tmpDo.setSrcBillType(tmpData[j++]);
            // �Ƶ�ʱ��
            tmpDo.setCreateTime(tmpData[j++]);
            // ��Чʱ��
            tmpDo.setEffectiveTime(tmpData[j++]);
            // ������
            tmpDo.setApprover(tmpData[j++]);
            // �޸���
            tmpDo.setUpdateBy(tmpData[j++]);
            // �Ƶ�����
            tmpDo.setSysOrgCode(tmpData[j++]);
            // �ѹر�
            tmpDo.setIsClosed(stoi(tmpData[j++]));
            // �����������
            tmpDo.setApprovalResultType(tmpData[j++]);
            // ��������
            tmpDo.setBillDate(tmpData[j++]);
            // �Ƶ���
            tmpDo.setCreateBy(tmpData[j++]);
            // �������
            tmpDo.setApprovalRemark(tmpData[j++]);

            //
            cgthckDo.push_back(tmpDo);
        }
    }
    list<CgthckEntryDO> cgthckEntryDo;
    for (int i = 1; i < entryData.size(); i++)
    {
        CgthckEntryDO tmpEntryDo;
        auto tmpData = entryData[i];
        int j = 0;
        // ����
        tmpEntryDo.setMaterialId(tmpData[j++]);
        // ���κ�
        tmpEntryDo.setBatchNo(tmpData[j++]);
        // �ֿ�
        tmpEntryDo.setWarehouseId(tmpData[j++]);
        // ���뷽��
        tmpEntryDo.setStockIoDirection(tmpData[j++]);
        // ��Ӧ��
        tmpEntryDo.setSupplierId(tmpData[j++]);
        // ������λ
        tmpEntryDo.setUnitId(tmpData[j++]);
        // �Ƕ�����
        tmpEntryDo.setSwellQty(stod(tmpData[j++]));
        // ����
        tmpEntryDo.setQty(stod(tmpData[j++]));
        // ����ɱ�����
        tmpEntryDo.setExpense(stod(tmpData[j++]));
        // �ɱ�
        tmpEntryDo.setCost(stod(tmpData[j++]));
        // ��������
        tmpEntryDo.setSettleQty(stod(tmpData[j++]));
        // ˰��%
        tmpEntryDo.setTaxRate(stod(tmpData[j++]));
        // ��˰����
        tmpEntryDo.setPrice(stod(tmpData[j++]));
        // �ۿ���%
        tmpEntryDo.setDiscountRate(stod(tmpData[j++]));
        // ˰��
        tmpEntryDo.setTax(stod(tmpData[j++]));
        // ������
        tmpEntryDo.setSettleAmt(stod(tmpData[j++]));
        // �ѿ�Ʊ����
        tmpEntryDo.setInvoicedQty(stod(tmpData[j++]));
        // �ѿ�Ʊ���
        tmpEntryDo.setInvoicedAmt(stod(tmpData[j++]));
        // �Զ���1
        tmpEntryDo.setCustom1(tmpData[j++]);
        // Դ���¼��
        tmpEntryDo.setSrcNo(tmpData[j++]);
        // ��¼��
        tmpEntryDo.setEntryNo(tmpData[j++]);
        // �Զ���2
        tmpEntryDo.setCustom2(tmpData[j++]);
        // Դ����¼id
        tmpEntryDo.setSrcEntryId(tmpData[j++]);
        // Դ������
        tmpEntryDo.setSrcBillType(tmpData[j++]);
        // ��ע
        tmpEntryDo.setRemark(tmpData[j++]);
        // ���ݱ��
        tmpEntryDo.setBillNo(tmpData[j++]);
        // Դ��id
        tmpEntryDo.setSrcBillId(tmpData[j++]);

        // 
        cgthckEntryDo.push_back(tmpEntryDo);
    }
    // ����list<CgthckDO>
    int result = 0;
    for (auto& sub : cgthckDo)
    {
        CgthckDAO dao;
        result += dao.importData(sub);
    }
    // ����list<CgthckEntryDO>
    for (auto& sub : cgthckEntryDo)
    {
        CgthckDAO dao;
        result += dao.importData(sub);
    }
    return result;
}

ExportCgthckVO CgthckService::exportData(const ExportCgthckFileDTO& dto)
{
    // ������ѯ����
    CgthckDAO dao;
    CgthckDO cgthckDo;
    CgthckEntryDO cgthckEntryDo;

    // ����sql���Ĳ�ѯ����
    cgthckDo.setBillNo(dto.getBillNo());
    cgthckEntryDo.setBillNo(dto.getBillNo());

    // ������ȷ�Ĳ�ѯ����
    list<CgthckDO> listDo = dao.exportData(cgthckDo);
    list<CgthckEntryDO> listEntryDo = dao.exportData(cgthckEntryDo);

    // ����ĳ��� ---> ��ά����
    vector<vector<std::string>> data;
    vector<vector<std::string>> entryData;

    // ��ͷ ��������һ������
    vector<string> header{
        CharsetConvertHepler::ansiToUtf8("���������"),
        CharsetConvertHepler::ansiToUtf8("�Ƿ�������"),
        CharsetConvertHepler::ansiToUtf8("�Ƿ����Ƕ�"),
        CharsetConvertHepler::ansiToUtf8("��Ӧ��"),
        CharsetConvertHepler::ansiToUtf8("�ͻ�"),
        CharsetConvertHepler::ansiToUtf8("��Ʊ����"),
        CharsetConvertHepler::ansiToUtf8("ҵ����"),
        CharsetConvertHepler::ansiToUtf8("ҵ��Ա"),
        CharsetConvertHepler::ansiToUtf8("����⾭��"),
        CharsetConvertHepler::ansiToUtf8("�ɱ�"),
        CharsetConvertHepler::ansiToUtf8("������"),
        CharsetConvertHepler::ansiToUtf8("�ѽ�����"),
        CharsetConvertHepler::ansiToUtf8("�ѿ�Ʊ���"),
        CharsetConvertHepler::ansiToUtf8("�Ƿ���Ч"),
        CharsetConvertHepler::ansiToUtf8("����"),
        CharsetConvertHepler::ansiToUtf8("Դ��id"),
        CharsetConvertHepler::ansiToUtf8("��������"),
        CharsetConvertHepler::ansiToUtf8("���ݽ׶�"),
        CharsetConvertHepler::ansiToUtf8("Դ����"),
        CharsetConvertHepler::ansiToUtf8("�Ƿ��Զ�����"),
        CharsetConvertHepler::ansiToUtf8("��ע"),
        CharsetConvertHepler::ansiToUtf8("����ʵ��id"),
        CharsetConvertHepler::ansiToUtf8("������"),
        CharsetConvertHepler::ansiToUtf8("���ݱ��"),
        CharsetConvertHepler::ansiToUtf8("�Ƿ����"),
        CharsetConvertHepler::ansiToUtf8("Դ������"),
        CharsetConvertHepler::ansiToUtf8("�Ƶ�ʱ��"),
        CharsetConvertHepler::ansiToUtf8("��Чʱ��"),
        CharsetConvertHepler::ansiToUtf8("������"),
        CharsetConvertHepler::ansiToUtf8("�޸���"),
        CharsetConvertHepler::ansiToUtf8("�Ƶ�����"),
        CharsetConvertHepler::ansiToUtf8("�ѹر�"),
        CharsetConvertHepler::ansiToUtf8("�����������"),
        CharsetConvertHepler::ansiToUtf8("��������"),
        CharsetConvertHepler::ansiToUtf8("�Ƶ���"),
        CharsetConvertHepler::ansiToUtf8("�������"),
    };
    data.push_back(header);
    // �����ѯ����Ϊ��
    if (!listDo.empty())
    {
        for (auto& tmpDo : listDo)
        {
            vector<string> sub{
                tmpDo.getStockIoType(),
                to_string(tmpDo.getHasRp()),
                to_string(tmpDo.getHasSwell()),
                tmpDo.getSupplierId(),
                tmpDo.getCustomerId(),
                tmpDo.getInvoiceType(),
                tmpDo.getOpDept(),
                tmpDo.getOperator1(),
                tmpDo.getHandler(),
                to_string(tmpDo.getCost()),
                to_string(tmpDo.getSettleAmt()),
                to_string(tmpDo.getSettledAmt()),
                to_string(tmpDo.getInvoicedAmt()),
                to_string(tmpDo.getIsEffective()),
                tmpDo.getAttachment(),
                tmpDo.getSrcBillId(),
                tmpDo.getSubject(),
                tmpDo.getBillStage(),
                tmpDo.getSrcNo(),
                to_string(tmpDo.getIsAuto()),
                tmpDo.getRemark(),
                tmpDo.getBpmiInstanceId(),
                to_string(tmpDo.getIsVoided()),
                tmpDo.getBillNo(),
                to_string(tmpDo.getIsRubric()),
                tmpDo.getSrcBillType(),
                tmpDo.getCreateTime(),
                tmpDo.getEffectiveTime(),
                tmpDo.getApprover(),
                tmpDo.getUpdateBy(),
                tmpDo.getSysOrgCode(),
                to_string(tmpDo.getIsClosed()),
                tmpDo.getApprovalResultType(),
                tmpDo.getBillDate(),
                tmpDo.getCreateBy(),
                tmpDo.getApprovalRemark()
            };
            data.push_back(sub);
        }
    }

    vector<string> entryHeader{
        CharsetConvertHepler::ansiToUtf8("����"),
        CharsetConvertHepler::ansiToUtf8("���κ�"),
        CharsetConvertHepler::ansiToUtf8("�ֿ�"),
        CharsetConvertHepler::ansiToUtf8("���뷽��"),
        CharsetConvertHepler::ansiToUtf8("��Ӧ��"),
        CharsetConvertHepler::ansiToUtf8("������λ"),
        CharsetConvertHepler::ansiToUtf8("�Ƕ�����+/-"),
        CharsetConvertHepler::ansiToUtf8("����"),
        CharsetConvertHepler::ansiToUtf8("����ɱ�����"),
        CharsetConvertHepler::ansiToUtf8("�ɱ�"),
        CharsetConvertHepler::ansiToUtf8("��������"),
        CharsetConvertHepler::ansiToUtf8("˰��%"),
        CharsetConvertHepler::ansiToUtf8("��˰����"),
        CharsetConvertHepler::ansiToUtf8("�ۿ���%"),
        CharsetConvertHepler::ansiToUtf8("˰��"),
        CharsetConvertHepler::ansiToUtf8("������"),
        CharsetConvertHepler::ansiToUtf8("�ѿ�Ʊ����"),
        CharsetConvertHepler::ansiToUtf8("�ѿ�Ʊ���"),
        CharsetConvertHepler::ansiToUtf8("�Զ���1"),
        CharsetConvertHepler::ansiToUtf8("Դ����¼��"),
        CharsetConvertHepler::ansiToUtf8("��¼��"),
        CharsetConvertHepler::ansiToUtf8("�Զ���2"),
        CharsetConvertHepler::ansiToUtf8("Դ����¼id"),
        CharsetConvertHepler::ansiToUtf8("Դ������"),
        CharsetConvertHepler::ansiToUtf8("��ע"),
        CharsetConvertHepler::ansiToUtf8("���ݱ��"),
        CharsetConvertHepler::ansiToUtf8("Դ��id")
    };
    data.push_back(entryHeader);

    if (!listEntryDo.empty())
    {
        for (auto& tempEntryDo : listEntryDo)
        {
            vector<string> sub
            {
                tempEntryDo.getMaterialId(),
                tempEntryDo.getBatchNo(),
                tempEntryDo.getWarehouseId(),
                tempEntryDo.getStockIoDirection(),
                tempEntryDo.getSupplierId(),
                tempEntryDo.getUnitId(),
                to_string(tempEntryDo.getSwellQty()),
                to_string(tempEntryDo.getQty()),
                to_string(tempEntryDo.getExpense()),
                to_string(tempEntryDo.getCost()),
                to_string(tempEntryDo.getSettleQty()),
                to_string(tempEntryDo.getTaxRate()),
                to_string(tempEntryDo.getPrice()),
                to_string(tempEntryDo.getDiscountRate()),
                to_string(tempEntryDo.getTax()),
                to_string(tempEntryDo.getSettleAmt()),
                to_string(tempEntryDo.getInvoicedQty()),
                to_string(tempEntryDo.getInvoicedAmt()),
                tempEntryDo.getCustom1(),
                tempEntryDo.getSrcNo(),
                tempEntryDo.getEntryNo(),
                tempEntryDo.getCustom2(),
                tempEntryDo.getSrcEntryId(),
                tempEntryDo.getSrcBillType(),
                tempEntryDo.getRemark(),
                tempEntryDo.getBillNo(),
                tempEntryDo.getSrcBillId()
            };
            data.push_back(sub);
        }
    }
    
    //���屣������λ�ú�ҳǩ����
    string fileName = "./public/excel/c5-cgthck.xlsx";
    string sheetName = CharsetConvertHepler::ansiToUtf8("����ⵥ");
 /*   string entrySheetName = CharsetConvertHepler::ansiToUtf8("��ϸ");*/

    // �����ļ�
    ExcelComponent excel;
    excel.writeVectorToFile(fileName, sheetName, data);
    //excel.writeVectorToFile(fileName, entrySheetName, entryData);

//    // ����fastdfs�ͻ��˶���
//#ifdef LINUX
//    FastDfsClient client("conf/client.conf", 3);
//#else
//    FastDfsClient client("1.15.240.108");
//#endif
//    // ���ļ��ϴ���fastdfs
//    string fieldName = client.uploadFile(fileName);
//    // ɾ�������ļ�
//    std::remove(fieldName.c_str());
//    // �������ص�ַ
//    fieldName = "http://1.15.240.108:8888/" + fieldName;

    ExportCgthckVO result(fileName);
    return result;
}
