{
  'targets': [
    {
      'target_name': 'cmakebuild',
      'target_type': 'none',
      'actions': [
        {
          'action_name': 'cmakejsaction',
          'inputs': ['CMakeLists.txt'],
          'outputs': ['build'],
          'conditions': [
              ['OS=="win"', {'action': ['node', '<!(echo %APPVEYOR_BUILD_FOLDER%)/compile.js']}],
              ['OS!="win"', {'action': ['node', 'compile.js']}]
          ]
        }
      ]
    }
  ]
}
