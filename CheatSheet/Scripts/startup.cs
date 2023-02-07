using UnityEngine;

public class startup : MonoBehaviour
{
    private void Start()
    {

    }

    private void Update()
    {

    }

    private void FixedUpdate()
    {

    }
}

public static class Utils
{
    //获取LocalPlayer
    static Handlers.GameHandlers.PlayerHandlers.LocalPlayer GetLocalPlayer()
    {
        //通过tag查找玩家
        GameObject[] gameObjects = GameObject.FindGameObjectsWithTag("Player");

        GameObject player = null;
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
            return null;
        }

        return player.GetComponent<Handlers.GameHandlers.PlayerHandlers.LocalPlayer>();
    }

    static Handlers.LobbyHandlers.LobbySceneHandler GetLobbySceneHandler()
    {
        //获取当前游戏场景中的Handlers
        GameObject handlers = GameObject.Find("Handlers");

        if (handlers == null)
        {
            return null;
        }

        //获取LobbySceneHandler
        Handlers.LobbyHandlers.LobbySceneHandler lobbySceneHandler = handlers.GetComponent<Handlers.LobbyHandlers.LobbySceneHandler>();

        if (lobbySceneHandler == null)
        {
            return null;
        }

        return lobbySceneHandler;
    }

    //放屁
    static void Fart()
    {
        GetLocalPlayer().SendFart();
    }

}