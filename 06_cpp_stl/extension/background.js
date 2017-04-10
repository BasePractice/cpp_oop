var port = browser.runtime.connectNative("ru.pastor.nm.example");

port.onMessage.addListener(function (response) {
    console.log("Received: " + response);
})
;

browser.browserAction.onClicked.addListener(function () {
    console.log("Sending:  command");
    port.postMessage({type: "command", property: "cmd", args: ["/k", "echo"]});
});