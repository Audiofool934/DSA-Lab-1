存储
- [x] 链表
- [ ] 哈希表

专利申请
“专利申请通过队列管理，按时间顺序处理”

企业申请专利时，申请信息进入队列排队。
专利按顺序出队进行审核，如果审核通过，则将此专利加入到企业的专利链表中。
未通过的审核结果都会记录到栈中，这些专利需要重新修改，同时优先处理最新的申请。

一个队列（可用链表实现），push pop，需要有专利合法检查，对了就进入储存系统，

不对则 进入栈（记录未通过审核的记录）

（按道理不应该进行**修改**，然后再进入专利合法检查的队尾？）

Q:
1. “优先处理”的道理 —— 历史记录（信息）放在栈里， 等待队列还是要放在队列里，合理
2. “修改”
3. 审核（判断逻辑？）（字符串匹配）
4. “待审核”（在开头统计好，和lab1加起来）



审核历史记录
申请排队
