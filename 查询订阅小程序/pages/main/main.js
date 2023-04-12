// pages/main.js
Component({

  properties: {

  },
  /**
   * 页面的初始数据
   */
  data: {
    place:"",
    // place:
    // [
    //   {
    //     place_names: "新工停车场",
    //     place_number: 200
    //   },
    //   {
    //     place_names: "新乡停车场",
    //     place_number: 600
    //   },
    //   {
    //     place_names: "郑州停车场",
    //     place_number: 900
    //   },
    //   {
    //     place_names: "开封停车场",
    //     place_number: 700
    //   },
    //   {
    //     place_names: "洛阳停车场",
    //     place_number: 450
    //   },
    // ],
    selected: 0,
    color: "#7A7E83",
    selectedColor: "#3cc51f",
    list: [{
      "pagePath": "/pages/main/main",
      "iconPath": "/image/首页.png",
      "selectedIconPath": "/image/首页.png",
      "text": "首页"
    }, {
      "pagePath": "/pages/set/set",
      "iconPath": "/image/我的.png",
      "selectedIconPath": "/image/我的.png",
      "text": "我的"
    }],
    
    

    //车牌号
    licence_plate :'',


    //测试使用
    //username :'',
    //pass :'',

  },
  methods: 
  {
    //当打开页面呈现的内容
    onShow() {
      if (typeof this.getTabBar === 'function' &&
      this.getTabBar()) 
      {
        this.getTabBar().setData({
          //唯一标识（其它设置不同的整数）  
          selected: 0
        })
      };
       //调用API读取停车场名和停车位传递给data的place
       wx.request
      ({
        url: 'http://36.133.121.177:66/connect_get.php', //上传预约信息
        data:
        {
        name:'36_133_121_177',
        password:'5211314',
        database:'36_133_121_177',
        },
        header: {
          'content-type': 'application/json;'
        },
        success:(res)=>
        {
          var place = JSON.parse(decodeURIComponent(res.data))
          //console.log(place)
          console.log("读取数据成功")
          this.setData
          (
            {
              "place":place,
            }
          )
        },
        fail:(f)=>
        {
          wx.showModal
          ({
            title: '提示',
            content: '读取数据失败',
          })
        }
      })
      wx.getStorage( //异步获取存储
        {
          key:'licence_plate', //读取本地缓存
          success:(res)=>{
            console.log('获取本地缓冲成功',res.data)
            this.setData(
              {
                "licence_plate":res.data,
              }
            )
          }
  
        },
      )
    },
  
    /**
     * 生命周期函数--监听页面隐藏
     */
    onHide() {
  
    },
  
    /**
     * 生命周期函数--监听页面卸载
     */
    onUnload() {
    },
  
    /**
     * 页面相关事件处理函数--监听用户下拉动作
     */
    onPullDownRefresh() {
  
    },
  
    /**
     * 页面上拉触底事件的处理函数
     */
    onReachBottom() {
  
    },
  
    /**
     * 用户点击右上角分享
     */
    onShareAppMessage() {
  
    },
    switchTab(e) {
      //console.log("执行跳转", e);
      const data = e.currentTarget.dataset
      const url = data.path
      wx.switchTab({
        url
      })
      this.setData({
        selected: data.index
      })
    },
    readusername(res)
      {
        this.setData(
          {
            username:res.detail.value
          }
        )
      },
    readpassword(res)
      {
        this.setData(
          {
            pass:res.detail.value
          }
        )
    },
    connectMysqlup_input:function(e)
  {
    if(this.data.licence_plate=='')
    {
      console.log("车牌号不存在需要跳转");
      wx.switchTab({
        url: '/pages/set/set',
      })
      wx.showToast({
        title: '已为你跳转，预定请先输入车牌号 ~ ~ ~',
        icon: 'none',
        duration: 2000//持续的时间
      })
      
    }
    else
    {
      const index = e.currentTarget.dataset.index
      const place = this.data.place[index]
      const placeName = place.place_names
      //console.log(`点击了 ${placeName} 的按钮`)
      wx.request({
        url: 'http://36.133.121.177:66/connect_up.php', //上传预约信息
        data:{
          name:'36_133_121_177',
          password:'5211314',
          database:'36_133_121_177',
          licence_plate:this.data.licence_plate, //上传车牌号
          licence_name : placeName,  //上传选中的停车场
        },
        success:(res)=>
        {
          console.log(res.data)
          wx.showModal({
            title: '成功',
            content: '预定成功，请您半小时后达到，否则将自动取消！',
            success: function (res) {
              if (res.confirm) { //这里是点击了确定以后
                console.log('用户点击确定')
              } else { //这里是点击了取消以后
                console.log('用户点击取消')
              }
            }
          })
        },
        fail:(f)=>{
          wx.showModal({
            title: '提示',
            content: '预定失败，数据库暂未连接',
            success: function (res) {
              if (res.confirm) { //这里是点击了确定以后
                console.log('用户点击确定')
              } else { //这里是点击了取消以后
                console.log('用户点击取消')
              }
            }
          })
          console.log(placeName,this.data.licence_plate);
          console.log('上传失败=====',f);
        }
      })
    }
    },

    connectmysqlget:function(e)
    {
      //调用API读取停车场名和停车位传递给data的place
     
    }

},
 
  
  
})




