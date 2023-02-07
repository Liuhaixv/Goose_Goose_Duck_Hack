//演示了查找本地玩家并放屁

//通过tag查找玩家
GameObject[] gameObjects = GameObject.FindGameObjectsWithTag("Player");

GameObject player;
foreach (GameObject gameObject in gameObjects)
{
    if (gameObject.GetComponent<Handlers.GameHandlers.PlayerHandlers.LocalPlayer>() == null)
    {
        continue;
    }
    else
    {
        player = gameObject;
        break;
    }
}

//未找到玩家实例
if (player == null)
{
    return;
}

//获取挂载的LocalPlayer
Handlers.GameHandlers.PlayerHandlers.LocalPlayer localPlayer = player.GetComponent<Handlers.GameHandlers.PlayerHandlers.LocalPlayer>();
//放屁
localPlayer.SendFart();