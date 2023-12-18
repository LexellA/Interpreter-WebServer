var bot = new ChatSDK({
  config: {
    navbar: {
      title: '智能助理'
    },
    robot: {
      avatar: '//gw.alicdn.com/tfs/TB1U7FBiAT2gK0jSZPcXXcKkpXa-108-108.jpg'
    },
    user: {
      avatar: '//gw.alicdn.com/tfs/TB1DYHLwMHqK1RjSZFEXXcGMXXa-56-62.svg',
    },
    messages: [
      {
        type: 'text',
        content: {
          text: '智能助理为您服务，请问有什么可以帮您？'
        }
      }
    ]
  },
  requests: {
    /**
     *
     * 问答接口
     * @param {object} msg - 消息
     * @param {string} msg.type - 消息类型
     * @param {string} msg.content - 消息内容
     * @return {object}
     */
    send: function (msg) {
      const data = msg.content;

      // 发送文本消息时
      if (msg.type === 'text') {
        return {
          url: '/chat/send',
          data: {
            q: data.text,
          },
        };
      }

    },
  }
});

bot.run();