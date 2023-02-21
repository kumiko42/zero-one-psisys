package com.zeroone.star.paymentmanagement.utils;

import com.zeroone.star.paymentmanagement.entity.PurchasePaymentEntity;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentCreateDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentUpdateDTO;

import java.lang.reflect.Field;

/**
 * @author Tomcat
 * @Date 2023/2/20 16:43
 */
public class Transformation {
    public static PurchasePaymentEntity transmissionCreateDTO(PaymentCreateDTO createDTO) throws IllegalAccessException {
        PurchasePaymentEntity payment = new PurchasePaymentEntity();
        Field[] declaredFields = payment.getClass().getDeclaredFields();
        Field[] fields = createDTO.getClass().getDeclaredFields();
        /*遍历数据传输类*/
        for (Field field : fields) {
            field.setAccessible(true);
            String name = field.getName();
            Object value = field.get(createDTO);
            /*遍历实体类寻找和该字段一样的字段*/
            for (Field field1 : declaredFields) {
                /*判断是否为一样的字段*/
                if (name.equals(field1.getName())) {
                    /*是的话设置值并跳出本层循环*/
                    field1.setAccessible(true);
                    field1.set(payment,value);
                    break;
                }
            }
        }
        return payment;
    }

    public static PurchasePaymentEntity transmissionUpdateDTO(PaymentUpdateDTO updateDTO) throws IllegalAccessException {
        PurchasePaymentEntity payment = new PurchasePaymentEntity();
        Field[] declaredFields = payment.getClass().getDeclaredFields();
        Field[] fields = updateDTO.getClass().getDeclaredFields();
        /*遍历数据传输类*/
        for (Field field : fields) {
            field.setAccessible(true);
            String name = field.getName();
            Object value = field.get(updateDTO);
            /*遍历实体类寻找和该字段一样的字段*/
            for (Field field1 :
                    declaredFields) {
                /*判断是否为一样的字段*/
                if (name.equals(field1.getName())) {
                    /*是的话设置值并跳出本层循环*/
                    field1.setAccessible(true);
                    field1.set(payment,value);
                    break;
                }
            }
        }
        return payment;
    }
}
