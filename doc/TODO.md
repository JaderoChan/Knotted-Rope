# TODO

- [x] 实现Config存储以下信息

  - 上一次打开的文件
  - 最近打开的文件列表
  - 默认文件存储位置
  - 程序语言
  - 是否自动打开上一次使用的文件

- [x] 实现Config中存储的信息相关功能
- [x] 实现Setting界面
- [x] 实现About界面
- [x] 实现Ropes界面（2025-11-06 02:47）
- [x] 实现托盘菜单
- [x] 实现`导出为CSV`功能（2025-11-05 14:20）（受限于CSV格式，在content中包含换行符时会导致混乱。此外由于文本内容存储为UTF-8格式，会导致以本地编码格式读取内容的Excel显示乱码）
- [ ] 实现绑定指定程序以记录特定程序使用情况功能
- [x] 增加暂停后自动保存功能（2025-11-06 03:05）
- [x] KnottedRope文件增加`InputContentWhenRopeFinished`以指示此KnottedRope文件是否会在新增Rope时弹出输入框输入content
- [ ] 新增柱状图显示功能，以天为单位查看每日耗时
- [x] 生成一个json测试性能
- [x] 更改Ropes界面，使用TableView进行Ropes显示
- [ ] 增加全局快捷键
- [x] 增加KnottedRopes属性修改界面
- [x] 修复系统托盘菜单在Knotted Rope名为空时不显示默认ToolTip文本的问题
- [x] 优化RecentFile的更新与添加时机
- [x] 修复RecentFile相关的逻辑问题
