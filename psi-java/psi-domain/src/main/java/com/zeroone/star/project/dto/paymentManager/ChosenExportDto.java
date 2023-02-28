package com.zeroone.star.project.dto.paymentManager;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import lombok.Data;

import java.util.List;

@Data
@ApiModel("导出数据功能的数据传输对象")
public class ChosenExportDto {
    @ApiModelProperty(value = "需要导出数据的id集合")
    private List<String> ids;
}
