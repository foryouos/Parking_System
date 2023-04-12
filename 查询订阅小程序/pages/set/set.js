// pages/set/set.js
Component({
  properties: {

  },
  /**
   * 页面的初始数据
   */
  data: {
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
    //存储输入框的值
    inputValue: ''
  },


  methods: {
    onLoad(options) {
      wx.getStorage( //异步获取存储
        {
          key:'licence_plate', //读取本地缓存
          success:(res)=>{
            console.log('获取本地缓冲成功',res.data)
            this.setData(
              {
                "inputValue":res.data,
              }
            )
          }
  
        }
      ) 
    },
    /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady() {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow() {
    if (typeof this.getTabBar === 'function' &&
    this.getTabBar()) {
    this.getTabBar().setData({
      //唯一标识（其它设置不同的整数）  
      selected: 1
    })
  }
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
    inputCom(e)
    {
      this.setData(
        {
          inputValue: e.detail.value
        }
      )
      console.log(this.data.inputValue);
    },
    //点击事件，将车牌号数据上传到本地
    doSearch()
    {
      console.log('search',this.data.inputValue);
      //将用户首次输入的数据放到车牌号中
      wx.setStorage(
        {
          key:'licence_plate',  //本地缓存中指定的key
          data:this.data.inputValue,   //需要存储的车牌号
          success:(s)=>{
            wx.showToast({
              title: '车牌保存成功~',
              icon: 'none',
              duration: 2000//持续的时间
            })
            console.log('存储缓冲成功=====',s);
          },
          fail:(f)=>{
            wx.showToast({
              title: '车牌保存失败~',
              icon: 'none',
              duration: 2000//持续的时间
            })
            console.log('存储缓存失败=====',f);
          }
        }
      )
    },
  

  },


  //在input输入框的bindinput时间
 
  /**
   * 生命周期函数--监听页面加载
   * 存放读取本地数据函数
   * 将页面信息存储到本地函数
   */


  

  
})





