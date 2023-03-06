import { 
    getBillList,    //查询单据列表
    getDetailBill,    //查看指定单据
    getPrepaybill,     //采购订单列表
    getPrepaybillDetail,    //采购订单分列表
    postAdd,      //添加采购预付申请单
    putModifyPay,    //修改采购预付申请
    deleteMenu,     //删除采购预付申请单
    modifyBillStatus,    //采购预付申请修改单据状态(关闭/反关闭/作废)
    payInto,     //导入
    payExport    //导出
} from '@/views/caigouguanli/api/purprepaymentapply.js'



export function handleGetBillList() {
  // 自定义请求参数
  let params = {
    pageIndex:0,
    pageSize:0,
    bill_no:"", 
    bill_begin_date:"",
    bill_end_date:"",
    subject: "",
	  supplier_id: "",
	  bill_stage: "",
  	is_effective: "-1",
	  is_closed: "-1",
	  is_voided: "-1"
  }
  params.pageIndex = 1
  params.pageSize = 10
  let pagniation={}
  getBillList(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handleGetDetailBill() {
  // 自定义请求参数
  let params = {
    bill_no:""
  }
  let pagniation={}
  getDetailBill(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handleGetPrepaybill() {
  // 自定义请求参数
  let params = {
    bill_no:"",
    bill_date:""
  }
  let pagniation={}
  getPrepaybill(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handleGetPrepaybillDetail() {
  // 自定义请求参数
  let params = {
    bill_no:""
  }
  let pagniation={}
  getPrepaybillDetail(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handlePostAdd() {
  // 自定义请求参数
  let params = {
    bill_no: "123456711",          //单据编号
	  bill_date: "2023-5-5",      //单据日期
	  subject: "采购笔记本",     //单据主题
	  op: "李业武",             //业务员
	  op_dept: "市场部",         //业务部门
	  supplier_id: "广电总局",      //供应商
    is_auto:0,                //是否自动生成（0为否，1为是）
    is_rubric:0,            //是否为红字单据（0为否，1为是）
    bill_stage:"编制中",    //如果点保存，就是“编制中”；如果点提交，就是“编制完”
    version:0,                //版本号
	  remark:"附件备注",        //附件备注
	  attachment: "xxx",         //附件
    detail: [
      {
        entry_no: "9",              //分录号
        src_bill_type: "采购",        //源单类型
        src_bill_id: "159999999999",   //源单ID
        src_no: "CGDD-221120-005",         //原单编号
        amt: 55555.50,               //申请金额
        paid_amt: 5555.40,         //已付金额
        remark: "",               //备注
        custom1: "",               //自定义1
        custom2: ""                 //自定义2
      },
      {
        entry_no: "10",
        src_bill_type: "采购",
        src_bill_id: "158888888888",
        src_no: "CGDD-221120-006",
        amt: 22222.10,
        paid_amt: 4444.10,
        remark: "",
        custom1: "",
        custom2: ""
      }
    ]
  }
  let pagniation={}
  postAdd(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handlePutModifyPay() {        //数据若完全与数据库一致无法修改
  // 自定义请求参数
  let params = {
    id: "12243087515380236288",           //单据ID
	  bill_date: "2023-5-5",                  //单据日期
	  subject: "采购笔记本",          //单据主题
	  op: "李业武",                 //业务员
	  op_dept: "市场部",           //业务部门
	  supplier_id: "广电总局",        //供应商
    is_auto:0,                //是否自动生成（0为否，1为是）
    is_rubric:0,            //是否为红字单据（0为否，1为是）
    bill_stage:"编制中",    //如果点保存，就是“编制中”；如果点提交，就是“编制完”
    version:0,                //版本号
	  remark:"附件备注",         //附件备注
	  attachment: "xxx",          //附件
    detail: [
      {
        entry_no: "9",
        src_bill_type: "采购",
        src_bill_id: "159999999999",
        src_no: "CGDD-221120-005",
        amt: 55555.50,
        paid_amt: 5555.40,
        remark: "",
        custom1: "",
        custom2: ""
      },
      {
        entry_no: "10",
        src_bill_type: "采购",
        src_bill_id: "158888888888",
        src_no: "CGDD-221120-006",
        amt: 22222.10,
        paid_amt: 4444.10,
        remark: "",
        custom1: "",
        custom2: ""
      }
    ]
  }
  let pagniation={}
  putModifyPay(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handleDeleteMenu() {
  // 自定义请求参数
  let params = {
    id: "1594317750844637186",
    bill_no:"CGYFSQ-221120-001"
  }
  let pagniation={}
  deleteMenu(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}

export function handleModifyBillStatus() {
  // 自定义请求参数
  let params = {
    id: "1594317750844637186",
    bill_no:"CGYFSQ-221120-001",
    opType:1
  }
  let pagniation={}
  modifyBillStatus(
    params,
    (data) => {
      console.log('全部请求data----', data)
      // 查询返回的是表格数据
      // 分页
      let pagination = {}
      pagination.value.currentPage = data.pageIndex
      pagination.value.pageSize = data.pageSize
      pagination.value.total = data.total
      let tableData = {}
      // 表格数据
      tableData.value = data.rows
    },
    (msg) => {
      let ElMessage = {}
      //ElMessage.warning(msg)
    }
  )
}
