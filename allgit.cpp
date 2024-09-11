/*
git clone https://github.com/devnomad517/test.git
git remote add origin https://github.com/devnomad517/test.git
git remote -v
git add .
git commit -m "initial commit"
git push -u origin main
gh auth login
git checkout main
git pull origin main
git checkout -b fix/1
echo "a" >> hello.text
git add .
git commit -m "Append letter a, commited as 1"
git push -u origin fix/1
gh pr create --title "Append letter a" --body "This pull request appends the letter a to hello.txt" --base main --head fix/1
gh issue create --title "Your issue title" --body "Description of the issue."
gh issue close 2
*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <algorithm>

void openAndAppendToFile(const std::string& filename, const std::string& textToAppend) {
    std::ofstream file(filename, std::ios_base::app); // Open file in append mode
    if (file.is_open()) {
        file << textToAppend; // Append text
        file.close();
        std::cout << "Appended text to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
}

void runGitCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Command failed: " << command << std::endl;
        throw std::runtime_error("Git command failed");
    }
}

std::string getPreviousMonthDate() {
    std::time_t now = std::time(0);
    std::tm* localtm = std::localtime(&now);

    // Go to previous month
    localtm->tm_mon -= 1; // Decrease month by 1
    std::mktime(localtm);  // Normalize the tm structure

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm); // Format date
    return std::string(buffer);
}
/*
void createPullRequest(const std::string& branchName) {
    std::string command = "gh pr create --title \"Append letter a\" --body \"This pull request appends the letter a to hello.txt\" --base main --head " + branchName;
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Failed to create pull request!" << std::endl;
        throw std::runtime_error("Pull request creation failed");
    }
}

void createIssue() {
    std::string command = "gh issue create --title \"New Issue\" --body \"This issue was created via C++\"";
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Failed to create issue!" << std::endl;
        throw std::runtime_error("Issue creation failed");
    }
}

void reviewPullRequest(int prNumber) {
    std::string command = "gh pr review --approve --body \"Looks good to me!\" --pr " + std::to_string(prNumber);
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Failed to review pull request!" << std::endl;
        throw std::runtime_error("Pull request review failed");
    }
}

void mergePullRequest(int prNumber) {
    std::string command = "gh pr merge " + std::to_string(prNumber) + " --merge";
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Failed to merge pull request!" << std::endl;
        throw std::runtime_error("Pull request merge failed");
    }
}

void closeIssue(int issueNumber) {
    std::string command = "gh issue close " + std::to_string(issueNumber);
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Failed to close issue!" << std::endl;
        throw std::runtime_error("Issue closing failed");
    }
}

std::string getLatestPullRequestNumber() {
    std::string command = "gh pr list --json number --limit 1 --jq \".[0].number\""; 
    char buffer[128];
    std::string result;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Failed to get latest PR number!" << std::endl;
        return "";
    }

    while (fgets(buffer, sizeof buffer, pipe.get()) != nullptr) {
        result += buffer;
    }

    // Trim the result string
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    
    return result;
}

*/
bool branchExists(const std::string& branchName) {
    std::string command = "git branch --list " + branchName;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    char buffer[128];
    std::string result;
    
    while (fgets(buffer, sizeof buffer, pipe.get()) != nullptr) {
        result += buffer;
    }
    
    return !result.empty();
}

int main() {
    const std::string filename = "hello.txt";
    const std::string textToAppend = "a\n"; // Character to append
    const std::string branchName = "feature/append-a"; // Branch name for PR
    
    try {
//        runGitCommand("gh auth login");

        // Step 1: Check if the branch exists, if not create it
        if (!branchExists(branchName)) {
            runGitCommand("git checkout -b " + branchName);
        } else {
            runGitCommand("git checkout " + branchName);
        }

        // Step 2: Append to hello.txt
        openAndAppendToFile(filename, textToAppend);

        // Step 3: Commit this change
        runGitCommand("git add .");
        runGitCommand("git commit -m \"Append letter a\"");

        // Step 4: Change the commit date to one month before
        std::string previousMonthDate = getPreviousMonthDate();
        runGitCommand("git commit --amend --no-edit --date=\"" + previousMonthDate + "\"");


        // Step 5: Push changes to the branch
        runGitCommand("git push --set-upstream origin " + branchName);
/*
        // Step 6: Create a pull request
        createPullRequest(branchName);

        // Step 7: Create an issue
        createIssue();

        // Step 8: Get and review the PR
        std::string prNumberStr = getLatestPullRequestNumber();
        if (!prNumberStr.empty()) {
            int prNumber = std::stoi(prNumberStr);
            reviewPullRequest(prNumber);
            mergePullRequest(prNumber);
        } else {
            std::cerr << "No pull requests found to review." << std::endl;
        }

        // Step 9: Close the issue (assuming issue number 1)
        closeIssue(1); // Replace with your actual issue number if necessary
*/
        // Step 10: Go back to the main branch for the next iteration
        runGitCommand("git checkout main");
        
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}
