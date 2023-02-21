package com.zeroone.star.paymentmanagement.service.Impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.paymentmanagement.entity.FinPaymentEntry;
import com.zeroone.star.paymentmanagement.entity.PurchasePaymentEntity;
import com.zeroone.star.paymentmanagement.mapper.FinPaymentEntryMapper;
import com.zeroone.star.paymentmanagement.mapper.PurchasePaymentMapper;
import com.zeroone.star.paymentmanagement.service.PPurchasePaymentService;
import com.zeroone.star.paymentmanagement.utils.Transformation;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentCreateDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentDeleteDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentUpdateDTO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * @author Tomcat
 * @Date 2023/2/15 22:36
 */
@Service
public class PurchasePaymentServiceImpl implements PPurchasePaymentService {
    @Autowired
    private PurchasePaymentMapper mapper;

    @Autowired
    private FinPaymentEntryMapper entryMapper;

    @Autowired
    private ObjectMapper objectMapper;

    @Override
    @Transactional(rollbackFor = {IllegalAccessException.class, RuntimeException.class})
    public int addPurchase(PaymentCreateDTO createDTO) throws IllegalAccessException {
        /*获取付款单明细集合*/
        String finPaymentEntryList = createDTO.getFinPaymentEntryList();

        /*将DTO转换为Entity*/
        PurchasePaymentEntity payment = Transformation.transmissionCreateDTO(createDTO);

        int result = 0;

        /*判断付款单数组是否为空*/
        if ("[]".equals(finPaymentEntryList)){
            /*增加付款单*/
            return mapper.addPurchase(payment);
        }else {
            /*将json数组转换为对应的实体对象数组*/
            FinPaymentEntry[] finPaymentEntry;
            try {
                finPaymentEntry = objectMapper.readValue(finPaymentEntryList, FinPaymentEntry[].class);
            } catch (JsonProcessingException e) {
                throw new RuntimeException(e);
            }

            /*增加付款单*/
            int i = mapper.addPurchase(payment);

            if (i!=1){
                result++;
            }

            /*循环插入付款单明细*/
            for (FinPaymentEntry entry : finPaymentEntry) {
                int insert = entryMapper.insert(entry);
                if (insert==1){
                    result++;
                }
            }
            return result;
        }
    }

    @Override
    @Transactional(rollbackFor = {IllegalAccessException.class,RuntimeException.class})
    public int updatePurhcase(PaymentUpdateDTO updateDTO) throws IllegalAccessException {
        /*获取付款单明细集合*/
        String finPaymentEntryList = updateDTO.getFinPaymentEntryList();

        /*将DTO转换为Entity*/
        PurchasePaymentEntity payment = Transformation.transmissionUpdateDTO(updateDTO);

        int result = 0;


        /*判断付款单数组是否为空*/
        if ("[]".equals(finPaymentEntryList)){
            /*更新付款单*/
            return mapper.updateById(payment);
        }else {
            /*将json数组转换为对应的实体对象数组*/
            FinPaymentEntry[] finPaymentEntry;

            try {
                finPaymentEntry = objectMapper.readValue(finPaymentEntryList, FinPaymentEntry[].class);
            } catch (JsonProcessingException e) {
                throw new RuntimeException(e);
            }

            /*更新付款单*/
            int i = mapper.updateById(payment);

            if (i != 1) {
                result++;
            }

            /*依据主表id查询所有明细*/
            QueryWrapper<FinPaymentEntry> wrapper = new QueryWrapper<>();
            wrapper.eq("mid",payment.getId());
            List<FinPaymentEntry> entries = entryMapper.selectList(wrapper);

            /*循环插入付款单明细*/
            for (FinPaymentEntry entry : finPaymentEntry) {
                /*遍历查询到的付款单明细*/
                for (FinPaymentEntry fin : entries) {
                    /*判断源单id是否相等，相等则已经存在*/
                    if (entry.getSrcBillId().equals(fin.getSrcBillId())) {
                        /*调用更新方法*/
                        int insert = entryMapper.updateById(entry);
                        if (insert == 1) {
                            result++;
                        }
                        break;
                    }
                }
                /*不相等调用插入方法*/
                int insert = entryMapper.insert(entry);
                if (insert == 1){
                    result++;
                }
            }
            return result;
        }
    }

    @Override
    public int deletePurchase(PaymentDeleteDTO deleteDTO) {
        String id = String.valueOf(deleteDTO.getId());
        PurchasePaymentEntity payment = mapper.selectById(id);
        payment.setIsVoided("1");
        return mapper.updateById(payment);
    }
}
