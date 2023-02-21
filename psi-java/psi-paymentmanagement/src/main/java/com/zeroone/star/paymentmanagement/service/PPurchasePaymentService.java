package com.zeroone.star.paymentmanagement.service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentCreateDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentDeleteDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentUpdateDTO;

/**
 * @author Tomcat
 * @Date 2023/2/15 22:36
 */
public interface PPurchasePaymentService {
    /**
     * 添加购买单据
     *
     * @param createDTO 创建dto
     * @return int 返回是否成功
     * @throws IllegalAccessException 非法访问异常
     */
    int addPurchase(PaymentCreateDTO createDTO) throws IllegalAccessException;

    /**
     * 更新采购情况
     *
     * @param updateDTO 更新dto
     * @return int 是否更新成功
     * @throws IllegalAccessException 非法访问异常
     */
    int updatePurhcase(PaymentUpdateDTO updateDTO) throws IllegalAccessException;

    /**
     * 逻辑删除购买单据
     *
     * @param deleteDTO 删除dto
     * @return int 返回是否成功
     */
    int deletePurchase(PaymentDeleteDTO deleteDTO);
}
