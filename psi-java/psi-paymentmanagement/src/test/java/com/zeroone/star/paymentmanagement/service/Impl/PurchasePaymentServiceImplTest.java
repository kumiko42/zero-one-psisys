package com.zeroone.star.paymentmanagement.service.Impl;

import com.zeroone.star.paymentmanagement.service.PPurchasePaymentService;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentUpdateDTO;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.Calendar;
import java.util.Date;

/**
 * @author Tomcat
 * @Date 2023/2/20 20:49
 */
@SpringBootTest
class PurchasePaymentServiceImplTest {
    @Autowired
    private PPurchasePaymentService paymentService;

    @Test
    void updatePurhcase() throws IllegalAccessException {
        Date date = new Date(2023, Calendar.FEBRUARY,19);
        Date date1 = new Date(2023,Calendar.MARCH,24);
        PaymentUpdateDTO paymentUpdateDTO = new PaymentUpdateDTO("test","CGFK-230215-038","","psi","0","0","FinPayment","",date,"","null","",date1,"0","0","A01A05",date1,"psi","","null","12","null","null","123456","1628040859106684930","0","null","2021","1623600860981665793","88885","0","88885","采购付款(有申请)","公交卡","管理员","否","否","否","否","研发部","管理员","编制中","否","[]");
        int i = paymentService.updatePurhcase(paymentUpdateDTO);
        System.out.println(i);
    }
}