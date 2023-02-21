package com.zeroone.star.paymentmanagement.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.ToString;

import java.util.Date;

import static com.baomidou.mybatisplus.annotation.IdType.ASSIGN_ID;

/**
 * @author Tomcat
 * @Date 2023/2/15 23:32
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@ToString
@TableName("fin_payment")
public class PurchasePaymentEntity {
    @TableId(type=ASSIGN_ID)
    private String id;
    private String remark;
    private String billNo;
    private String approvalResultType;
    private String updateBy;
    private String isEffective;
    private String isVoided;
    private String approver;
    private Date billDate;
    private String srcBillType;
    private String approvalRemark;
    private String srcNo;
    private Date updateTime;
    private String isAuto;
    private String isRubric;
    private String sysOrgCode;
    private Date createTime;
    private String createBy;
    private String srcBillId;
    private String effectiveTime;
    private String billStage;
    private String version;
    private String attachment;
    private String subject;
    private String isClosed;
    private String bpmiInstanceId;
    private String paymentType;
    private String supplierId;
    private String amt;
    private String checkedAmt;
}
