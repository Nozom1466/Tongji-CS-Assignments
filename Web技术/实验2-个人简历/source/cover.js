const cachedTheme = localStorage.getItem("__theme");

if (cachedTheme === undefined) {
  cachedTheme = "light";
  localStorage.setItem("__theme", "light");
}

if (cachedTheme === "dark") {
  document.body.classList.add("dark");
}

const theme = document.querySelectorAll(".home__theme");
theme.forEach((t) =>
  t.addEventListener("click", () => {
    if (document.body.classList.contains("dark")) {
      document.body.classList.remove("dark");
      localStorage.setItem("__theme", "light");
    } else {
      document.body.classList.add("dark");
      localStorage.setItem("__theme", "dark");
    }
  })
);

let th = 0;
function func() {
  let divtextList = [
    "Tongji Student",
    "Creative",
    "Regoroius",
    "Industrious",
    "OK!!!",
  ];

  let divText = divtextList[th];
  let len = divText.length;
  let divNode = document.getElementById("brief-intro-content-j");
  let i = 0;
  let f = 0;
  let coding = setInterval(function () {
    if (f == 0) {
      if (i < len) {
        divNode.innerHTML = divText.slice(0, i) + "|";
      } else {
        if (i == len) {
          divNode.innerHTML = divText.slice(0, i);
          f = 1;
        }
        // clearInterval(coding);
      }
      i++;
    } else if (f == 1) {
      if (i <= len && i >= 2) {
        divNode.innerHTML = divText.slice(0, i) + "|";
      } else {
        if (i < 2) {
          divNode.innerHTML = divText.slice(0, i);
          f = 0;
        }
        // clearInterval(coding);
      }
      i--;
    }

    // console.log(i);
  }, 180);
}

func();
