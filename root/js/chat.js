var bot = new ChatSDK({
  config: {
    navbar: {
      title: '客服机器人'
    },
    robot: {
      avatar: './avatar.jpg',
    },
    user: {
      avatar: '//gw.alicdn.com/tfs/TB1DYHLwMHqK1RjSZFEXXcGMXXa-56-62.svg',
    },
    
  },
  requests: {
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