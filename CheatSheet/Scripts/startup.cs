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

    static Handlers.GameHandlers.TaskHandlers.TasksHandler GetTasksHandler()
    {
        //获取当前游戏场景中的Handlers
        GameObject handlers = GameObject.Find("Handlers");

        if (handlers == null)
        {
            return null;
        }

        //获取LobbySceneHandler
        Handlers.GameHandlers.TaskHandlers.TasksHandler tasksHandler = handlers.GetComponent<Handlers.GameHandlers.TaskHandlers.TasksHandler>();

        if (tasksHandler == null)
        {
            return null;
        }

        return tasksHandler;
    }

    //完成所有任务
    static void CompleteAllTasks()
    {
        //1.获取TasksHandler
        //2.获取assignedTasks
        //3.获取assignedTasks中的entries
        //4.获取entries的count，即任务数量
        //5.遍历entries，key即为TaskId
        //6.使用TasksHandler.CompleteTask(string taskId, false, false, false, true)完成任务

        Handlers.GameHandlers.TaskHandlers.TasksHandler tasksHandler = Utils.GetTasksHandler();

        //Dictionary<string GameTask>
        var assignedTasks = tasksHandler.assignedTasks;

        foreach (string taskId in assignedTasks.Keys)
        {
            tasksHandler.CompleteTask(taskId, false, false, false, true);
        }
    }

    //放屁
    static void Fart()
    {
        GetLocalPlayer().SendFart();
    }

}