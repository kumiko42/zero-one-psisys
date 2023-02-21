package com.zeroone.star.paymentmanagement.controller;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.zeroone.star.paymentmanagement.service.PPurchasePaymentService;
import com.zeroone.star.project.components.fastdfs.FastDfsClientComponent;
import com.zeroone.star.project.components.fastdfs.FastDfsFileInfo;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentCreateDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentDeleteDTO;
import com.zeroone.star.project.dto.paymentmanagement.purchasepayment.PaymentUpdateDTO;
import com.zeroone.star.project.paymentmanagement.purchasepayment.PurchasePaymentApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.annotation.Resource;

/**
 * @author Tomcat
 * @Date 2023/2/15 22:22
 */
@RestController
@RequestMapping("/purchase")
@Api(tags = "付款管理-采购付款")
@Validated
public class PurchasePaymentController implements PurchasePaymentApis {
    @Autowired
    private PPurchasePaymentService paymentService;

    @Resource
    private FastDfsClientComponent fastDfsClientComponent;

    @Value("${fastdfs.nginx-servers}")
    private String urlPrefix;

    @PostMapping("/save")
    @ApiOperation("采购付款增加")
    @Override
    public JsonVO<String> save(PaymentCreateDTO create) throws IllegalAccessException{
        int purchase = paymentService.addPurchase(create);
        if (purchase>0){
            return JsonVO.success("新增保存成功！");
        }else {
            return JsonVO.fail("新增保存失败！");
        }
    }

    @DeleteMapping("/delete")
    @ApiOperation("采购付款删除")
    @Override
    public JsonVO<String> delete(PaymentDeleteDTO delete) {
        int update = paymentService.deletePurchase(delete);
        if (update>0){
            return JsonVO.success("删除成功！");
        }else {
            return JsonVO.fail("删除失败！");
        }
    }

    @PutMapping("/update")
    @ApiOperation("采购付款修改")
    @Override
    public JsonVO<String> update(PaymentUpdateDTO update) throws IllegalAccessException {
        int purchase = paymentService.updatePurhcase(update);
        if (purchase>0){
            return JsonVO.success("编辑保存成功!");
        }else {
            return JsonVO.fail("编辑保存失败！");
        }
    }

    @PostMapping("/upload")
    @ApiOperation("附件上传")
    @Override
    public JsonVO<String> upload(MultipartFile flie) throws Exception {
        String originalFilename = flie.getOriginalFilename();
        String substring = originalFilename.substring(originalFilename.lastIndexOf(".") + 1);
        FastDfsFileInfo info = fastDfsClientComponent.uploadFile(flie.getBytes(), substring);
        if (info == null){
            return JsonVO.fail("上传文件失败");
        }
        return JsonVO.success(fastDfsClientComponent.fetchUrl(info,urlPrefix,true));
    }
}
