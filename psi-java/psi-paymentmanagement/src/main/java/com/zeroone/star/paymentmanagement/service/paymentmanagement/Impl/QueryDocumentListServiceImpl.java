package com.zeroone.star.paymentmanagement.service.paymentmanagement.Impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.paymentmanagement.entity.QueryDocumentList;
import com.zeroone.star.paymentmanagement.mapper.QueryDocumentListMapper;
import com.zeroone.star.paymentmanagement.service.paymentmanagement.IQueryDocumentListService;
import com.zeroone.star.project.query.paymentmanagement.QueryDocumentListQuery;
import com.zeroone.star.project.vo.PageVO;
import com.zeroone.star.project.vo.paymentmanagement.QueryDocumentListVO;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
@Service
public class QueryDocumentListServiceImpl extends ServiceImpl<QueryDocumentListMapper, QueryDocumentList> implements IQueryDocumentListService {

    @Resource
    private QueryDocumentListMapper mapper;

    @Override
    public PageVO<QueryDocumentListVO> queryDocumentList(QueryDocumentListQuery query) {
        //构建分页对象
        Page<QueryDocumentList> QueryDocumentListPage = new Page<>(query.getPageIndex(), query.getPageSize());
        // 执行查询
        Page<QueryDocumentList> result = mapper.queryDocumentList(QueryDocumentListPage,query);
        return PageVO.create(result, QueryDocumentListVO.class);
    }
}
