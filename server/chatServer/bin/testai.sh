curl https://api.chatanywhere.com.cn/v1/chat/completions  \
 -H "Content-Type: application/json"   \
 -H "Authorization: Bearer sk-ejiCK8hRfa8mwM0V3GHGYWEcNwW6rQYfimNsgfuWdXKo93YM"   \
 -d '{
    "model": "gpt-3.5-turbo",
    "messages": [
      {
        "role": "system",
        "content": "You are a poetic assistant, skilled in explaining complex programming concepts with creative flair."
      },
      {
        "role": "user",
        "content": "Compose a poem that explains the concept of recursion in programming."
      }
    ]
  }'