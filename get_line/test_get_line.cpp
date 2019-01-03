#include <cstdlib>
#include "gtest/gtest.h"
#include "get_line.h"

/*
 * Some lines of a large test file that will be created in memory
 */
#define LINE0 "A test.\n"
#define LINE1 "A paragraph should ideally be a smooth, succinct experience that goes through a bit of exposition, illustrates an idea, sums up the point, and primes the reader for the next paragraph.\n"
#define LINE2 "Ideally.\n"
#define LINE3 "\n"
#define LINE4 "In practice, a writer can get too caught up in all the things they have to say and fail to organize it all into bits an ordinary human being would be able to digest. The end result is a huge run-on paragraph that makes it difficult to recall the original point of it, if there was one in the first place. The reader's eyes glaze over and all they see is a wall of text.\n"
#define LINE5 "This afflicts all written media, but it is particularly infamous for its effect on Comic Books. One of the first things learned in comics is how to use dialogue bubbles effectively; a writer not allocating space carefully will end up covering their panel with a bunch of text and white space. Eventually the reader will realize that they're just looking at plain text rather than the vivid form of storytelling by imagery that comic books are famed for. TL;DR indeed.\n"
#define LINE6 "At best, a Wall of Text is just a signal of really heavy exposition. At worst, they are a warning sign that the author is soapboxing about something.\n"
#define LINE7 "Speaking in Panels is often a way to evade this trope while recounting what happened.\n"
#define LINE8 "If Speech-Bubbles Interruption are used to show it's not being listened to, see Wall of Blather. If the text is literally written on a wall in-universe, it might be a Room Full of Crazy. See Read the Fine Print if these kinds of text actually contain very important information. Ominous Multiple Screens is sort-of the video equivalent.\n"
#define LINE9 "For egregious examples right in This Very Wiki, observe the venerable entrants of Trope Overdosed.\n"
#define LINEA "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but because occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure? On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammelled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains. But I must explain to you how"

#define NUM_LINES 11

/*
 * Test fixture for testing read_line()
 */
class ReadLineTest : public testing::Test {
protected:

    virtual void SetUp() override
    {
        m_file_handle = nullptr;
        m_file_contents = nullptr;
        m_file_contents_statically_allocated = true;
        m_readline_buffer = nullptr;
        m_readline_buffer_size = 0;
        m_basic_file_contents = strdup("This is a test\nfile with 2 lines.");
    }

    virtual void TearDown() override
    {
        CloseFileAndFreeMemoryBufferIfNotStaticallyAllocated();
        free(m_readline_buffer);
        free(m_basic_file_contents);
    }

    void CloseFileAndFreeMemoryBufferIfNotStaticallyAllocated()
    {
        if (m_file_handle)
            fclose(m_file_handle);

        if (!m_file_contents_statically_allocated)
            free(m_file_contents);
    }

    /*
     * To OpenManagedFileFromStaticMemoryBuffer we just need to fmemopen() the
     * buffer, remember __not__ to free() the buffer when we die but still close the file handle.
     */
    void OpenManagedFileFromStaticMemoryBuffer(char *const file_contents, const size_t file_size)
    {
        ASSERT_NE(file_contents, nullptr);
        ASSERT_NE(file_size, 0);

        CloseFileAndFreeMemoryBufferIfNotStaticallyAllocated();
        m_file_contents_statically_allocated = true;
        m_file_contents = nullptr;

        m_file_handle = fmemopen(file_contents, file_size, "r");
        ASSERT_NE(m_file_handle, nullptr);
    }

    /*
     * To OpenManagedFileFromAllocatedMemoryBufferAndFreeOnExit we just need to fmemopen() the
     * buffer, remember __to__ free() the buffer when we die and close the file handle.
     */
    void OpenManagedFileFromAllocatedMemoryBufferAndFreeOnExit(char *const file_contents, const size_t file_size)
    {
        ASSERT_NE(file_contents, nullptr);
        ASSERT_NE(file_size, 0);

        CloseFileAndFreeMemoryBufferIfNotStaticallyAllocated();
        m_file_contents_statically_allocated = false;
        m_file_contents = file_contents;

        m_file_handle = fmemopen(file_contents, file_size, "r");
        ASSERT_NE(m_file_handle, nullptr);
    }

    void RequestThatReadLineAllocatesLineBufferForUser()
    {
        if (m_readline_buffer) {
            free(m_readline_buffer);
        }
        m_readline_buffer = nullptr;
        m_readline_buffer_size = 0;
    }

    void ProvideReadLineWithUserAllocatedBuffer(const size_t buffer_size)
    {
        if (m_readline_buffer) {
            free(m_readline_buffer);
        }
        m_readline_buffer = (char *)malloc(buffer_size);
        ASSERT_NE(m_readline_buffer, nullptr);
        m_readline_buffer_size = buffer_size;
    }

    int CallReadLineAndCheck(const int expected_result, const char *const expected_buffer, const size_t expected_buffer_size)
    {
        const int result = read_line(m_file_handle, &m_readline_buffer, &m_readline_buffer_size);
        EXPECT_EQ(result, expected_result);
        if (expected_buffer) {
            EXPECT_NE(m_readline_buffer, nullptr);
            EXPECT_STREQ(m_readline_buffer, expected_buffer);
        }
        else {
            EXPECT_EQ(m_readline_buffer, nullptr);
        }
        EXPECT_GT(m_readline_buffer_size, expected_buffer_size);
    }

    void TestBasicFileContentsRead()
    {
        CallReadLineAndCheck(
            ResultIs(1),
            BufferContentIs("This is a test\n"),
            BufferSizeIsGreaterThan(14));

        CallReadLineAndCheck(
            ResultIs(1),
            BufferContentIs("file with 2 lines."),
            BufferSizeIsGreaterThan(17));

        CallReadLineAndCheck(
            ResultIs(0),
            BufferContentIs(""),
            BufferSizeIsGreaterThan(17));
    }

    char *ConstructLargeFileMemoryBuffer()
    {
        char *new_file_buffer = nullptr;

        /* Calculate accumlated string length of lines in m_large_file_expected_lines[] */
        size_t text_size = 0;
        for (int i = 0; i < NUM_LINES; ++i) {
            text_size += strlen(m_large_file_expected_lines[i]);
        }

        /* Create and populate one large continous character array from all the lines
         * in m_large_file_expected_lines[] */
        new_file_buffer = (char *)malloc(text_size + 1);
        //ASSERT_NE(new_file_buffer, nullptr);

        size_t text_size_so_far = 0;
        for (int i = 0; i < NUM_LINES; ++i) {
            strcpy(new_file_buffer + text_size_so_far, m_large_file_expected_lines[i]);
            text_size_so_far += strlen(m_large_file_expected_lines[i]);
        }
        EXPECT_EQ(text_size_so_far, text_size);

        return new_file_buffer;
    }

    inline int ResultIs(const int value)
    {
        return value;
    }

    inline const char *BufferContentIs(const char *const buffer)
    {
        return buffer;
    }

    inline size_t BufferSizeIsGreaterThan(const size_t buffer_size)
    {
        return buffer_size;
    }


    FILE *m_file_handle;
    char *m_file_contents;
    bool m_file_contents_statically_allocated;

    char *m_readline_buffer;
    size_t m_readline_buffer_size;

    char *m_basic_file_contents;
    static const char *m_large_file_expected_lines[NUM_LINES];
};

const char *ReadLineTest::m_large_file_expected_lines[NUM_LINES] = {
    LINE0, LINE1, LINE2, LINE3, LINE4, LINE5, LINE6, LINE7, LINE8, LINE9, LINEA};

TEST_F(ReadLineTest, AutoBufferAllocationFileRead)
{
    OpenManagedFileFromStaticMemoryBuffer(m_basic_file_contents, strlen(m_basic_file_contents) + 1);
    RequestThatReadLineAllocatesLineBufferForUser();
    TestBasicFileContentsRead();
}

TEST_F(ReadLineTest, SmallUserDefinedBufferResizedFileRead)
{
    OpenManagedFileFromStaticMemoryBuffer(m_basic_file_contents, strlen(m_basic_file_contents));
    ProvideReadLineWithUserAllocatedBuffer(1);
    TestBasicFileContentsRead();
}

TEST_F(ReadLineTest, LargeBuffer)
{
    char *const new_file_buffer = ConstructLargeFileMemoryBuffer();
    OpenManagedFileFromAllocatedMemoryBufferAndFreeOnExit(new_file_buffer, strlen(new_file_buffer));

    RequestThatReadLineAllocatesLineBufferForUser();
    size_t max_line_size_so_far = 0;
    for (int i = 0; i < NUM_LINES; ++i) {
        SCOPED_TRACE(i);

        const size_t line_size = strlen(m_large_file_expected_lines[i]);
        if (max_line_size_so_far < line_size)
            max_line_size_so_far = line_size;

        CallReadLineAndCheck(
            ResultIs(1),
            BufferContentIs(m_large_file_expected_lines[i]),
            BufferSizeIsGreaterThan(max_line_size_so_far - 1));
    }

    CallReadLineAndCheck(
        ResultIs(0),
        BufferContentIs(""),
        BufferSizeIsGreaterThan(max_line_size_so_far - 1));

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
